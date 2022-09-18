#include "Parser.h"

// TODO: Remove nop
#define nop

char RegenParser::scanEscape(std::string::iterator &iter, const std::string::iterator &endIter)
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

RegenParser::tokenType RegenParser::scanToken(std::string::iterator &iter, const std::string::iterator &endIter, std::stack<contextType> &ctxStack)
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
    auto cmpStackTop = [](const std::stack<contextType> &stack, enum contextType ctx)
    {
        return !stack.empty() && stack.top() == ctx;
    };

    switch (*iter)
    {
    //? CHCLASS_BEGIN handler placeholder
    case '[':
        ctxStack.push(contextType::CHCLASS);
        setOffNType(1, tokenType::CHCLASS_BEGIN);
        break;

    //? CHCLASS_END handler placeholder
    case ']':
        if (!cmpStackTop(ctxStack, contextType::CHCLASS))
            throw RegenException::ContextMismatchException();
        else
        {
            ctxStack.pop();
            setOffNType(1, tokenType::CHCLASS_END);
        }
        break;

    //? CHCLASS_SEP handler placeholder
    case '-':
        if (!cmpStackTop(ctxStack, RegenParser::contextType::CHCLASS))
            setOffNType(0, tokenType::UNDEFINED);
        else
            setOffNType(1, tokenType::CHCLASS_RANGE);
        break;

    //? GROUP_BEGIN handler placeholder
    case '(':
        if (cmpStackTop(ctxStack, contextType::CHCLASS))
            setOffNType(0, tokenType::UNDEFINED);
        else
        {
            ctxStack.push(contextType::GROUP);
            setOffNType(1, tokenType::GROUP_BEGIN);
        }
        break;

    //? GROUP_END handler placeholder
    case ')':
        if (cmpStackTop(ctxStack, contextType::CHCLASS))
            setOffNType(0, tokenType::UNDEFINED);
        else
        {
            if (!cmpStackTop(ctxStack, contextType::GROUP))
                throw RegenException::ContextMismatchException();
            else
                setOffNType(1, tokenType::GROUP_END);
        }
        break;

    //? GROUP_SEP handler placeholder
    case '|':
        if (!cmpStackTop(ctxStack, contextType::GROUP))
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

std::shared_ptr<RegenAST::ASTNode> RegenParser::CreateLiteralNode(std::shared_ptr<RegenAST::ASTNode> nodeRef, int id, std::string str)
{
    auto childRef = std::make_shared<RegenAST::ASTNode>(id, RegenAST::nodeType::LITERAL, nodeRef);
    childRef.get()->Data.SetLiteral(str);
    nodeRef.get()->GetChildren().emplace_back(childRef);

    return childRef;
}

std::shared_ptr<RegenAST::ASTNode> RegenParser::CreateChClassNode(std::shared_ptr<RegenAST::ASTNode> nodeRef, int id)
{
    auto childRef = std::make_shared<RegenAST::ASTNode>(id, RegenAST::nodeType::CHCLASS, nodeRef);
    nodeRef.get()->GetChildren().emplace_back(childRef);

    return childRef;
}

void RegenParser::NormalizeAST(const std::vector<std::shared_ptr<RegenAST::ASTNode>> &nodeRefVec)
{
    for (const auto &nodeRef : nodeRefVec)
    {
        RegenAST::nodeType type = nodeRef.get()->Data.GetNodeType();

        if (type == RegenAST::nodeType::ENTRY)
            continue;

        if (nodeRef.get()->Data.Empty())
        {
            std::shared_ptr<RegenAST::ASTNode> parentRef = nodeRef.get()->GetParent();
            // parentRef.get()->GetChildren().pop_back();
            parentRef.get()->GetChildren().remove(nodeRef);
            for (auto &childRef : nodeRef.get()->GetChildren())
            {
                childRef.get()->setParent(parentRef);
                parentRef.get()->GetChildren().emplace_back(childRef);
            }
        }
    }
}

std::shared_ptr<RegenAST::ASTNode> RegenParser::parseExpression(std::string &expression)
{
    // TODO: Should also handle the empty expression case

    std::string literalBuff;
    std::stack<RegenParser::contextType> ctxStack;
    std::vector<std::shared_ptr<RegenAST::ASTNode>> nodeRefVec;
    auto rootRef = std::make_shared<RegenAST::ASTNode>(0, RegenAST::nodeType::ENTRY, nullptr);
    std::shared_ptr<RegenAST::ASTNode> nodeRef = rootRef;
    int currId = 0;

    // TODO: Remove setParent call
    rootRef.get()->setParent(rootRef);

    char lastChClassCh = '\0';
    bool chClassRangeFlag = false;
    bool createChClassNodeFlag = false;

    auto iter = expression.begin();
    auto endIter = expression.end();
    while (iter != expression.end())
    {
        tokenType type = RegenParser::scanToken(iter, endIter, ctxStack);

        if (type == tokenType::UNDEFINED)
        {
            char escapeCh = scanEscape(iter, endIter);
            char ch = escapeCh == -1 ? *iter : escapeCh;

            lastChClassCh = ch;
            literalBuff += ch;
            iter++;
        }
        else
        {
            chClassRangeFlag = false;
            createChClassNodeFlag = false;

            switch (type)
            {
            case tokenType::CHCLASS_BEGIN:
                createChClassNodeFlag = true;
                break;

            case tokenType::CHCLASS_END:
                if (!literalBuff.empty())
                {
                    nodeRef.get()->Data.SetChSet(literalBuff);
                    literalBuff.clear();
                }
                else
                {
                    if (RegenOutput::OUTPUT_ENABLED)
                        RegenOutput::CerrWarning(RegenOutput::CHCLASS_EMPTY_WARNING);
                }
                break;

            case tokenType::CHCLASS_RANGE:
                chClassRangeFlag = true;
                break;

            default:
                break;
            }

            if (!literalBuff.empty())
            {
                if (chClassRangeFlag)
                {
                    nodeRef.get()->Data.SetChSet(lastChClassCh, *iter);
                    chClassRangeFlag = false;
                }
                else
                {
                    currId++;
                    nodeRef = CreateLiteralNode(nodeRef, currId, literalBuff);
                    nodeRefVec.push_back(nodeRef);
                    literalBuff.clear();
                }
            }

            if (createChClassNodeFlag)
            {
                currId++;
                nodeRef = CreateChClassNode(nodeRef, currId);
                nodeRefVec.push_back(nodeRef);
            }
        }
    }

    if (!literalBuff.empty())
    {
        currId++;
        CreateLiteralNode(nodeRef, currId, literalBuff);
        nodeRefVec.push_back(nodeRef);
        nodeRefVec.push_back(nodeRef);
        literalBuff.clear();
    }

    if (chClassRangeFlag)
        throw RegenException::InvalidRangeException();

    if (!ctxStack.empty())
        throw RegenException::ContextMismatchException();

    NormalizeAST(nodeRefVec);

    return rootRef;
}