#include "Parser.h"

// TODO: Remove nop
#define nop

char RegenParser::Parser::_scanEscape(std::string::iterator &iter, const std::string::iterator &endIter)
{
    if (*iter != '\\' || iter + 1 == endIter)
        return -1;

    iter++;
    switch (*iter)
    {
    case '0':
        return '\0';
    case 't':
        return '\t';
    case 'r':
        return '\r';
    case 'n':
        return '\n';
    default:
        return *iter;
    }
}

RegenParser::tokenType RegenParser::Parser::_scanToken(std::string::iterator &iter, const std::string::iterator &endIter, std::stack<contextGroup> &ctxStack)
{
    short iterOff;
    enum tokenType type;

    //? CHCLASS_DIGIT/CHCLASS_WORD/CHCLASS_SPACE handler placeholder
    if (*iter == '\\')
    {
        if (iter + 1 == endIter)
            return RegenParser::tokenType::UNDEFINED;

        iterOff = 2;
        switch (*(iter + 1))
        {
        case 'd':
            type = tokenType::CHCLASS_DIGIT;
            break;
        case 'w':
            type = tokenType::CHCLASS_WORD;
            break;
        case 's':
            type = tokenType::CHCLASS_SPACE;
            break;
        case 'D':
            type = tokenType::NCHCLASS_DIGIT;
            break;
        case 'W':
            type = tokenType::NCHCLASS_WORD;
            break;
        case 'S':
            type = tokenType::NCHCLASS_SPACE;
            break;
        default:
            iterOff = 0;
            type = tokenType::UNDEFINED;
        }

        iter += iterOff;
        return type;
    }

    /// @brief Shorthand for iterOff and type assignments
    auto setOffNType = [&](short newIterOff, enum tokenType newType)
    {
        iterOff = newIterOff;
        type = newType;
    };

    /// @brief Shorthand for the stack context checking condition
    auto cmpStackTop = [](const std::stack<contextGroup> &stack, enum contextGroup ctx)
    {
        return !stack.empty() && stack.top() == ctx;
    };

    switch (*iter)
    {
    //? CHCLASS_BEGIN handler placeholder
    case '[':
        if (iter + 1 < endIter && *(iter + 1) == '^')
            setOffNType(2, tokenType::NCHCLASS_BEGIN);
        else
            setOffNType(1, tokenType::CHCLASS_BEGIN);
        ctxStack.push(contextGroup::CHCLASS);
        break;

    //? CHCLASS_END handler placeholder
    case ']':
        if (!cmpStackTop(ctxStack, contextGroup::CHCLASS))
            throw RegenException::ContextMismatchException();
        else
        {
            setOffNType(1, tokenType::CHCLASS_END);
            ctxStack.pop();
        }
        break;

    //? CHCLASS_SEP handler placeholder
    case '-':
        if (!cmpStackTop(ctxStack, RegenParser::contextGroup::CHCLASS))
            setOffNType(0, tokenType::UNDEFINED);
        else
            setOffNType(1, tokenType::CHCLASS_RANGE);
        break;

    //? GROUP_BEGIN handler placeholder
    case '(':
        if (cmpStackTop(ctxStack, contextGroup::CHCLASS))
            setOffNType(0, tokenType::UNDEFINED);
        else
        {
            ctxStack.push(contextGroup::GROUP);
            setOffNType(1, tokenType::GROUP_BEGIN);
        }
        break;

    //? GROUP_END handler placeholder
    case ')':
        if (cmpStackTop(ctxStack, contextGroup::CHCLASS))
            setOffNType(0, tokenType::UNDEFINED);
        else
        {
            if (!cmpStackTop(ctxStack, contextGroup::GROUP))
                throw RegenException::ContextMismatchException();
            else
                setOffNType(1, tokenType::GROUP_END);
        }
        break;

    //? GROUP_SEP handler placeholder
    case '|':
        if (!cmpStackTop(ctxStack, contextGroup::GROUP))
            setOffNType(0, tokenType::UNDEFINED);
        else
            setOffNType(1, tokenType::GROUP_SEP);
        break;

    //? UNDEFINED handler placeholder
    default:
        setOffNType(0, tokenType::UNDEFINED);
    }

    iter += iterOff;
    return type;
}

std::shared_ptr<RegenAST::ASTNode> RegenParser::Parser::_createLiteralNode(std::shared_ptr<RegenAST::ASTNode> nodeRef, int id, std::string str)
{
    auto childRef = std::make_shared<RegenAST::ASTNode>(id, RegenAST::NodeType::LITERAL, nodeRef);
    childRef.get()->GetDataRef().SetLiteral(str);
    nodeRef.get()->GetChildrenRef().emplace_back(childRef);

    return childRef;
}

std::shared_ptr<RegenAST::ASTNode> RegenParser::Parser::_createChClassNode(std::shared_ptr<RegenAST::ASTNode> nodeRef, int id)
{
    auto childRef = std::make_shared<RegenAST::ASTNode>(id, RegenAST::NodeType::CHCLASS, nodeRef);
    nodeRef.get()->GetChildrenRef().emplace_back(childRef);

    return childRef;
}

void RegenParser::Parser::_normalizeAST(const std::vector<std::shared_ptr<RegenAST::ASTNode>> &nodeRefVec)
{
    for (const auto &nodeRef : nodeRefVec)
    {
        RegenAST::NodeType type = nodeRef.get()->GetDataRef().GetNodeType();

        if (type == RegenAST::NodeType::ENTRY)
            continue;

        if (nodeRef.get()->GetDataRef().Empty())
        {
            std::shared_ptr<RegenAST::ASTNode> parentRef = nodeRef.get()->GetParentRef();

            //? Might be inefficient
            parentRef.get()->GetChildrenRef().remove(nodeRef);

            for (auto &childRef : nodeRef.get()->GetChildrenRef())
            {
                childRef.get()->SetParent(parentRef);
                parentRef.get()->GetChildrenRef().emplace_back(childRef);
            }
        }
    }
}

std::shared_ptr<RegenAST::ASTNode> RegenParser::Parser::ParseExpression(std::string &expression)
{
    // TODO: Should also handle the empty expression case

    std::string literalBuff;
    std::stack<RegenParser::contextGroup> ctxStack;
    std::vector<std::shared_ptr<RegenAST::ASTNode>> nodeRefVec;
    auto rootRef = std::make_shared<RegenAST::ASTNode>(0, RegenAST::NodeType::ENTRY, nullptr);
    std::shared_ptr<RegenAST::ASTNode> nodeRef = rootRef;
    int currId = 0;

    // TODO: Remove setParent call
    rootRef.get()->SetParent(rootRef);

    char lastChClassCh = '\0';
    bool chClassRangeFlag = false;
    bool createNodeFlag = false;
    tokenType prevType = tokenType::UNDEFINED;

    auto iter = expression.begin();
    auto endIter = expression.end();
    while (iter != expression.end())
    {
        tokenType type = RegenParser::Parser::_scanToken(iter, endIter, ctxStack);

        if (type == tokenType::UNDEFINED)
        {
            // TODO: Determine other way to return error for scanEscape
            char escapeCh = _scanEscape(iter, endIter);
            char ch = escapeCh == -1 ? *iter : escapeCh;

            lastChClassCh = ch;
            literalBuff += ch;
            iter++;
        }
        else
        {
            switch (type)
            {
            case tokenType::CHCLASS_BEGIN:
                createNodeFlag = true;
                prevType = tokenType::CHCLASS_BEGIN;
                break;

            case tokenType::CHCLASS_END:
                if (!literalBuff.empty())
                {
                    if (prevType == RegenParser::tokenType::CHCLASS_BEGIN)
                        nodeRef.get()->GetDataRef().SetChSet(literalBuff);
                    else if (prevType == RegenParser::tokenType::NCHCLASS_BEGIN)
                        nodeRef.get()->GetDataRef().SetChSet(literalBuff, true);
                    literalBuff.clear();
                }
                else
                {
                    if (prevType == RegenParser::tokenType::CHCLASS_BEGIN && RegenOutput::OUTPUT_ENABLED)
                        RegenOutput::CerrWarning(RegenOutput::CHCLASS_EMPTY_WARNING);
                    if (prevType == RegenParser::tokenType::NCHCLASS_BEGIN && RegenOutput::OUTPUT_ENABLED)
                        RegenOutput::CerrWarning(RegenOutput::NCHCLASS_EMPTY_WARNING);
                }
                prevType = tokenType::UNDEFINED;
                break;

            case tokenType::NCHCLASS_BEGIN:
                createNodeFlag = true;
                prevType = tokenType::NCHCLASS_BEGIN;
                break;

            case tokenType::CHCLASS_RANGE:
                chClassRangeFlag = true;
                break;

            case tokenType::CHCLASS_DIGIT:
                createNodeFlag = true;
                break;

            case tokenType::CHCLASS_WORD:
                createNodeFlag = true;
                break;

            case tokenType::CHCLASS_SPACE:
                createNodeFlag = true;
                break;

            case tokenType::NCHCLASS_DIGIT:
                createNodeFlag = true;
                break;

            case tokenType::NCHCLASS_WORD:
                createNodeFlag = true;
                break;

            case tokenType::NCHCLASS_SPACE:
                createNodeFlag = true;
                break;

            default:
                break;
            }

            if (!literalBuff.empty())
            {
                if (chClassRangeFlag)
                {
                    if (prevType == tokenType::CHCLASS_BEGIN)
                        nodeRef.get()->GetDataRef().SetChSet(lastChClassCh, *iter);
                    else if (prevType == tokenType::NCHCLASS_BEGIN)
                        nodeRef.get()->GetDataRef().SetChSet(lastChClassCh, *iter, true);

                    chClassRangeFlag = false;
                }
                else
                {
                    currId++;
                    nodeRef = _createLiteralNode(nodeRef, currId, literalBuff);
                    nodeRefVec.push_back(nodeRef);
                    literalBuff.clear();
                }
            }

            if (createNodeFlag)
            {
                // TODO: Handle chclass inside chclass case ([abc[def]] and [^\w])
                // TODO: Implement chclass intersection token
                //? Possible request user to use \W instead of [^\w]
                if (nodeRef.get()->GetDataRef().GetNodeType() != RegenAST::NodeType::CHCLASS)
                    nop;

                currId++;
                nodeRef = _createChClassNode(nodeRef, currId);
                if (prevType == tokenType::NCHCLASS_BEGIN)
                    nodeRef.get()->GetDataRef().FillChSet(true);

                switch (type)
                {
                case RegenParser::tokenType::CHCLASS_DIGIT:
                    nodeRef.get()->GetDataRef().SetChSet('0', '9');
                    break;

                case RegenParser::tokenType::CHCLASS_WORD:
                    nodeRef.get()->GetDataRef().SetChSet('a', 'z');
                    nodeRef.get()->GetDataRef().SetChSet('A', 'Z');
                    nodeRef.get()->GetDataRef().SetChSet('0', '9');
                    nodeRef.get()->GetDataRef().SetChSet('_');
                    break;

                case RegenParser::tokenType::CHCLASS_SPACE:
                    nodeRef.get()->GetDataRef().SetChSet(' ');
                    nodeRef.get()->GetDataRef().SetChSet('\n');
                    nodeRef.get()->GetDataRef().SetChSet('\t');
                    break;

                case RegenParser::tokenType::NCHCLASS_DIGIT:
                    nodeRef.get()->GetDataRef().FillChSet(true);
                    nodeRef.get()->GetDataRef().SetChSet('0', '9', true);
                    break;

                case RegenParser::tokenType::NCHCLASS_WORD:
                    nodeRef.get()->GetDataRef().FillChSet(true);
                    nodeRef.get()->GetDataRef().SetChSet('a', 'z', true);
                    nodeRef.get()->GetDataRef().SetChSet('A', 'Z', true);
                    nodeRef.get()->GetDataRef().SetChSet('0', '9', true);
                    nodeRef.get()->GetDataRef().SetChSet('_', true);
                    break;

                case RegenParser::tokenType::NCHCLASS_SPACE:
                    nodeRef.get()->GetDataRef().FillChSet(true);
                    nodeRef.get()->GetDataRef().SetChSet(' ', true);
                    nodeRef.get()->GetDataRef().SetChSet('\n', true);
                    nodeRef.get()->GetDataRef().SetChSet('\t', true);
                    break;

                default:
                    break;
                }

                nodeRefVec.push_back(nodeRef);
                createNodeFlag = false;
            }
        }
    }

    if (!literalBuff.empty())
    {
        currId++;
        _createLiteralNode(nodeRef, currId, literalBuff);
        nodeRefVec.push_back(nodeRef);
        nodeRefVec.push_back(nodeRef);
        literalBuff.clear();
    }

    if (chClassRangeFlag)
        throw RegenException::InvalidRangeException();

    if (!ctxStack.empty())
        throw RegenException::ContextMismatchException();

    _normalizeAST(nodeRefVec);

    return rootRef;
}