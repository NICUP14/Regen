#include "Parser.h"

bool RegenParser::Parser::_scanEscape(std::string::iterator &iterRef, const std::string::iterator &endIterRef, char &escapeSeqRef)
{
    if (iterRef >= endIterRef)
        return false;

    if (*iterRef != '\\' || iterRef + 1 >= endIterRef)
        return false;

    iterRef++;
    switch (*iterRef)
    {
    //? Null escape sequence placeholder.
    case '0':
        escapeSeqRef = '\0';
        break;

    //? Bell/Alarm escape sequence placeholder.
    case 'a':
        escapeSeqRef = '\a';
        break;

    //? Backspace escape sequence placeholder.
    case 'b':
        escapeSeqRef = '\b';
        break;

    //? Newline escape sequence placeholder.
    case 'n':
        escapeSeqRef = '\n';
        break;

    //? Tab escape sequence placeholder.
    case 't':
        escapeSeqRef = '\t';
        break;

    //? Vertical tab escape sequence placeholder.
    case 'v':
        escapeSeqRef = '\v';
        break;

    //? Form feed escape sequence placeholder.
    case 'f':
        escapeSeqRef = '\f';
        break;

    //? Escape escape sequence placeholder.
    case 'e':
        escapeSeqRef = '\e';
        break;

    //? Carriage return escape sequence placeholder.
    case 'r':
        escapeSeqRef = '\r';
        break;

    default:
        escapeSeqRef = *iterRef;
    }

    return true;
}

RegenParser::TokenType RegenParser::Parser::_scanDefinedChClass(std::string::iterator &iterRef, const std::string::iterator &endIterRef)
{
    if (iterRef >= endIterRef)
        return TokenType::UNDEFINED;

    if (*iterRef != '\\' || iterRef + 1 >= endIterRef)
        return TokenType::UNDEFINED;

    static auto iterPointsTo = [&](const std::string::iterator &iter, char ch)
    {
        return iter < endIterRef && *iter == ch;
    };

    short iterOff = 2;
    TokenType tokenType;

    /// Detects non-compliant regex pre-defined character class constructs.
    if (!REGEN_REGEX_COMPLIANT)
    {
        switch (*(iterRef + 1))
        {
        //? CHCLASS_UPPER check placeholder
        case 'u':
            tokenType = TokenType::CHCLASS_UPPER;
            break;

        //? CHCLASS_LOWER check placeholder
        case 'l':
            tokenType = TokenType::CHCLASS_LOWER;
            break;

        //? CHCLASS_ALNUM/CHCLASS_ALPHA check placeholder
        case 'a':
            if (iterPointsTo(iterRef + 2, 'n'))
            {
                tokenType = TokenType::CHCLASS_ALNUM;
                iterOff++;
            }
            else
                tokenType = TokenType::CHCLASS_ALPHA;
            break;

        //? CHCLASS_PUNCT check placeholder
        case 'p':
            tokenType = TokenType::CHCLASS_PUNCT;
            break;

        //? CHCLASS_CLOSURE check placeholder
        case 'c':
            tokenType = TokenType::CHCLASS_CLOSURE;
            break;

        //? CHCLASS_MATH check placeholder
        case 'm':
            tokenType = TokenType::CHCLASS_MATH;
            break;

        //? NCHCLASS_UPPER check placeholder
        case 'U':
            tokenType = TokenType::NCHCLASS_UPPER;
            break;

        //? NCHCLASS_LOWER check placeholder
        case 'L':
            tokenType = TokenType::NCHCLASS_LOWER;
            break;

        //? NCHCLASS_ALNUM/NCHCLASS_ALPHA check placeholder
        case 'A':
            if (iterPointsTo(iterRef + 2, 'N'))
            {
                tokenType = TokenType::NCHCLASS_ALNUM;
                iterOff++;
            }
            else
                tokenType = TokenType::NCHCLASS_ALPHA;
            break;

        //? NCHCLASS_PUNCT check placeholder
        case 'P':
            tokenType = TokenType::NCHCLASS_PUNCT;
            break;

        //? NCHCLASS_CLOSURE check placeholder
        case 'C':
            tokenType = TokenType::NCHCLASS_CLOSURE;
            break;

        //? NCHCLASS_MATH check placeholder
        case 'M':
            tokenType = TokenType::NCHCLASS_MATH;
            break;

        default:
            tokenType = TokenType::UNDEFINED;
            break;
        }

        if (tokenType != TokenType::UNDEFINED)
        {
            iterRef += iterOff;
            return tokenType;
        }
    }

    /// Detects compliant regex pre-defined character class constructs.
    switch (*(iterRef + 1))
    {
    //? CHCLASS_DIGIT check placeholder
    case 'd':
        tokenType = TokenType::CHCLASS_DIGIT;
        break;

    //? CHCLASS_WORD check placeholder
    case 'w':
        tokenType = TokenType::CHCLASS_WORD;
        break;

    //? CHCLASS_SYMBOL/CHCLASS_SPACE check placeholder
    case 's':
        if (iterPointsTo(iterRef + 2, 'b'))
        {
            tokenType = TokenType::CHCLASS_SYMBOL;
            iterOff++;
        }
        else
            tokenType = TokenType::CHCLASS_SPACE;
        break;

    //? NCHCLASS_DIGIT check placeholder
    case 'D':
        tokenType = TokenType::NCHCLASS_DIGIT;
        break;

    //? NCHCLASS_WORD check placeholder
    case 'W':
        tokenType = TokenType::NCHCLASS_WORD;
        break;

    //? NCHCLASS_SYMBOL/NCHCLASS_SPACE check placeholder
    case 'S':
        if (iterPointsTo(iterRef + 2, 'B'))
        {
            tokenType = TokenType::NCHCLASS_SYMBOL;
            iterOff++;
        }
        else
            tokenType = TokenType::NCHCLASS_SPACE;
        break;

    default:
        return TokenType::UNDEFINED;
    }

    iterRef += iterOff;
    return tokenType;
}

RegenParser::TokenType RegenParser::Parser::_scanOperator(std::string::iterator &iterRef, const std::string::iterator &endIterRef, std::stack<RegenParser::ContextGroup> &ctxGrStackRef)
{
    short iterOff;
    TokenType tokenType;

    /// @brief Shorthand lambda for iterOff and type assignments
    auto setOffNType = [&](short newIterOff, enum TokenType newType)
    {
        iterOff = newIterOff;
        tokenType = newType;
    };

    /// @brief Shorthand lambda for the stack context checking condition
    auto cmpStackTop = [](const std::stack<ContextGroup> &stack, enum ContextGroup ctx)
    {
        return !stack.empty() && stack.top() == ctx;
    };

    switch (*iterRef)
    {
    //? CHCLASS_BEGIN check placeholder
    case '[':
        if (iterRef + 1 < endIterRef && *(iterRef + 1) == '^')
            setOffNType(2, TokenType::NCHCLASS_BEGIN);
        else
            setOffNType(1, TokenType::CHCLASS_BEGIN);
        ctxGrStackRef.push(ContextGroup::CHCLASS);
        break;

    //? CHCLASS_END check placeholder
    case ']':
        if (!cmpStackTop(ctxGrStackRef, ContextGroup::CHCLASS))
            throw RegenException::ContextMismatchException();
        setOffNType(1, TokenType::CHCLASS_END);
        ctxGrStackRef.pop();
        break;

    //? CHCLASS_SEP check placeholder
    case '-':
        if (cmpStackTop(ctxGrStackRef, RegenParser::ContextGroup::CHCLASS))
            return TokenType::UNDEFINED;
        setOffNType(1, TokenType::CHCLASS_RANGE);
        break;

    //? CHCLASS_AND check placeholder
    case '&':
        if (iterRef + 1 < endIterRef && *(iterRef + 1) == '&')
            setOffNType(2, TokenType::CHCLASS_AND);
        return TokenType::UNDEFINED;

    //? GROUP_BEGIN check placeholder
    case '(':
        if (cmpStackTop(ctxGrStackRef, ContextGroup::CHCLASS))
            return TokenType::UNDEFINED;
        ctxGrStackRef.push(ContextGroup::GROUP);
        setOffNType(1, TokenType::GROUP_BEGIN);
        break;

    //? GROUP_END check placeholder
    case ')':
        if (cmpStackTop(ctxGrStackRef, ContextGroup::CHCLASS))
            return TokenType::UNDEFINED;
        if (!cmpStackTop(ctxGrStackRef, ContextGroup::GROUP))
            throw RegenException::ContextMismatchException();
        setOffNType(1, TokenType::GROUP_END);
        break;

    //? GROUP_SEP check placeholder
    case '|':
        if (!cmpStackTop(ctxGrStackRef, ContextGroup::GROUP))
            return TokenType::UNDEFINED;
        setOffNType(1, TokenType::GROUP_SEP);
        break;

    default:
        return TokenType::UNDEFINED;
    }

    iterRef += iterOff;
    return tokenType;
}

RegenParser::TokenType RegenParser::Parser::_scanToken(std::string::iterator &iterRef, const std::string::iterator &endIterRef, std::stack<ContextGroup> &ctxGrStackRef)
{
    if (iterRef >= endIterRef)
        return TokenType::UNDEFINED;

    if (TokenType tokenType; (tokenType = _scanDefinedChClass(iterRef, endIterRef)) != TokenType::UNDEFINED)
        return tokenType;

    return _scanOperator(iterRef, endIterRef, ctxGrStackRef);
}

std::shared_ptr<RegenAST::ASTNode> RegenParser::Parser::_createLiteralNode(std::shared_ptr<RegenAST::ASTNode> nodeRef, int id, const std::string &str)
{
    auto childRef = std::make_shared<RegenAST::ASTNode>(id, RegenAST::NodeType::LITERAL, nodeRef);
    nodeRef.get()->GetChildrenRef().emplace_back(childRef);
    childRef.get()->GetDataRef().SetLiteral(str);

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
    // TODO: Implement transformations

    for (const auto &nodeRef : nodeRefVec)
    {
        /// Skips entry nodes. (safety check)
        if (RegenAST::NodeType type = nodeRef.get()->GetDataRef().GetNodeType();
            type == RegenAST::NodeType::ENTRY)
            continue;

        /// Removes the redundant node.
        if (nodeRef.get()->GetDataRef().Empty())
        {
            std::shared_ptr<RegenAST::ASTNode> parentRef = nodeRef.get()->GetParentRef();

            parentRef.get()->GetChildrenRef().remove(nodeRef);
            for (auto &childRef : nodeRef.get()->GetChildrenRef())
            {
                childRef.get()->SetParent(parentRef);
                parentRef.get()->GetChildrenRef().emplace_back(childRef);
            }
        }
    }
}

void RegenParser::Parser::_setAbsNodeTypeNFlags(RegenParser::TokenType tokenType, RegenParser::AbstractNodeType &absNodeTypeRef, bool &createNodeFlagRef, bool &chClassRangeFlagRef)
{
    if (tokenType == TokenType::CHCLASS_RANGE)
    {
        createNodeFlagRef = false;
        chClassRangeFlagRef = true;
        absNodeTypeRef = AbstractNodeType::LITERAL;
        return;
    }

    createNodeFlagRef = true;
    chClassRangeFlagRef = false;

    switch (tokenType)
    {
    case TokenType::CHCLASS_BEGIN:
        absNodeTypeRef = AbstractNodeType::CHCLASS;
        break;

    case TokenType::NCHCLASS_BEGIN:
        absNodeTypeRef = AbstractNodeType::NCHCLASS;
        break;

    case TokenType::CHCLASS_END:
        absNodeTypeRef = AbstractNodeType::LITERAL;
        break;

    case TokenType::CHCLASS_DIGIT:
        absNodeTypeRef = AbstractNodeType::CHCLASS_DIGIT;
        break;

    case TokenType::CHCLASS_WORD:
        absNodeTypeRef = AbstractNodeType::CHCLASS_WORD;
        break;

    case TokenType::CHCLASS_SPACE:
        absNodeTypeRef = AbstractNodeType::CHCLASS_SPACE;
        break;

    case TokenType::CHCLASS_LOWER:
        absNodeTypeRef = AbstractNodeType::CHCLASS_LOWER;
        break;

    case TokenType::CHCLASS_UPPER:
        absNodeTypeRef = AbstractNodeType::CHCLASS_ALPHA;
        break;

    case TokenType::CHCLASS_ALNUM:
        absNodeTypeRef = AbstractNodeType::CHCLASS_ALNUM;
        break;

    case TokenType::CHCLASS_SYMBOL:
        absNodeTypeRef = AbstractNodeType::CHCLASS_SYMBOL;
        break;

    case TokenType::CHCLASS_PUNCT:
        absNodeTypeRef = AbstractNodeType::CHCLASS_PUNCT;
        break;

    case TokenType::CHCLASS_CLOSURE:
        absNodeTypeRef = AbstractNodeType::CHCLASS_CLOSURE;
        break;

    case TokenType::CHCLASS_MATH:
        absNodeTypeRef = AbstractNodeType::CHCLASS_MATH;
        break;

    case TokenType::NCHCLASS_DIGIT:
        absNodeTypeRef = AbstractNodeType::NCHCLASS_DIGIT;
        break;

    case TokenType::NCHCLASS_WORD:
        absNodeTypeRef = AbstractNodeType::NCHCLASS_WORD;
        break;

    case TokenType::NCHCLASS_SPACE:
        absNodeTypeRef = AbstractNodeType::NCHCLASS_SPACE;
        break;

    case TokenType::NCHCLASS_LOWER:
        absNodeTypeRef = AbstractNodeType::NCHCLASS_LOWER;
        break;

    case TokenType::NCHCLASS_UPPER:
        absNodeTypeRef = AbstractNodeType::NCHCLASS_ALPHA;
        break;

    case TokenType::NCHCLASS_ALNUM:
        absNodeTypeRef = AbstractNodeType::NCHCLASS_ALNUM;
        break;

    case TokenType::NCHCLASS_SYMBOL:
        absNodeTypeRef = AbstractNodeType::NCHCLASS_SYMBOL;
        break;

    case TokenType::NCHCLASS_PUNCT:
        absNodeTypeRef = AbstractNodeType::NCHCLASS_PUNCT;
        break;

    case TokenType::NCHCLASS_CLOSURE:
        absNodeTypeRef = AbstractNodeType::NCHCLASS_CLOSURE;
        break;

    case TokenType::NCHCLASS_MATH:
        absNodeTypeRef = AbstractNodeType::NCHCLASS_MATH;
        break;

    case TokenType::CHCLASS_RANGE:
    default:
        createNodeFlagRef = false;
        chClassRangeFlagRef = false;
        break;
    }
}

std::shared_ptr<RegenAST::ASTNode> RegenParser::Parser::ParseExpression(std::string &expressionRef)
{
    // TODO: Implement character class intersections

    if (expressionRef.empty())
        return nullptr;

    int currId = 0;
    std::string literalBuff;
    std::stack<RegenParser::ContextGroup> ctxStack;
    std::vector<std::shared_ptr<RegenAST::ASTNode>> nodeRefVec;
    auto rootRef = std::make_shared<RegenAST::ASTNode>(0, RegenAST::NodeType::ENTRY, nullptr);
    std::shared_ptr<RegenAST::ASTNode> nodeRef = rootRef;

    //? The root (entry) node of the AST is circular. (root->Parent == root)
    rootRef.get()->SetParent(rootRef);

    char lastChClassCh = '\0';
    bool chClassRangeFlag;
    bool createNodeFlag;

    AbstractNodeType absNodeType;
    auto iter = expressionRef.begin();
    auto endIter = expressionRef.end();
    while (iter != expressionRef.end())
    {
        TokenType tokenType = RegenParser::Parser::_scanToken(iter, endIter, ctxStack);

        if (tokenType == TokenType::UNDEFINED)
        {
            char escapeSeq;
            char resultCh = _scanEscape(iter, endIter, escapeSeq) ? escapeSeq : *iter;

            lastChClassCh = resultCh;
            literalBuff += resultCh;
            iter++;
        }
        else
        {
            _setAbsNodeTypeNFlags(
                tokenType,
                absNodeType,
                createNodeFlag,
                chClassRangeFlag);

            if (chClassRangeFlag)
                nodeRef.get()->GetDataRef().SetChSet(lastChClassCh, *iter);

            if (!createNodeFlag)
                continue;

            currId++;
            nodeRef = _createChClassNode(nodeRef, currId);

            if (absNodeType == AbstractNodeType::LITERAL && !literalBuff.empty())
            {
                nodeRef.get()->GetDataRef().SetChSet(literalBuff);
                literalBuff.clear();
            }

            if (absNodeType >= AbstractNodeType::NCHCLASS)
            {
                nodeRef.get()->GetDataRef().FillChSet(true);
                nodeRef.get()->GetDataRef().FlipInvertFlag();
            }

            switch (absNodeType)
            {
            case AbstractNodeType::CHCLASS_DIGIT:
            case AbstractNodeType::NCHCLASS_DIGIT:
                nodeRef.get()->GetDataRef().SetChSet('0', '9');
                break;

            case AbstractNodeType::CHCLASS_WORD:
            case AbstractNodeType::NCHCLASS_WORD:
                nodeRef.get()->GetDataRef().SetChSet('a', 'z');
                nodeRef.get()->GetDataRef().SetChSet('A', 'Z');
                nodeRef.get()->GetDataRef().SetChSet('0', '9');
                nodeRef.get()->GetDataRef().SetChSet("_");
                break;

            case AbstractNodeType::CHCLASS_SPACE:
            case AbstractNodeType::NCHCLASS_SPACE:
                nodeRef.get()->GetDataRef().SetChSet(CHCLASS_SPACE_STR);
                break;

            case AbstractNodeType::CHCLASS_LOWER:
            case AbstractNodeType::NCHCLASS_LOWER:
                nodeRef.get()->GetDataRef().SetChSet('a', 'z');
                break;

            case AbstractNodeType::CHCLASS_UPPER:
            case AbstractNodeType::NCHCLASS_UPPER:
                nodeRef.get()->GetDataRef().SetChSet('A', 'Z');
                break;

            case AbstractNodeType::CHCLASS_ALPHA:
            case AbstractNodeType::NCHCLASS_ALPHA:
                nodeRef.get()->GetDataRef().SetChSet('a', 'z');
                nodeRef.get()->GetDataRef().SetChSet('A', 'Z');
                break;

            case AbstractNodeType::CHCLASS_ALNUM:
            case AbstractNodeType::NCHCLASS_ALNUM:
                nodeRef.get()->GetDataRef().SetChSet('a', 'z');
                nodeRef.get()->GetDataRef().SetChSet('A', 'Z');
                nodeRef.get()->GetDataRef().SetChSet('0', '9');
                break;

            case AbstractNodeType::CHCLASS_SYMBOL:
            case AbstractNodeType::NCHCLASS_SYMBOL:
                nodeRef.get()->GetDataRef().SetChSet(CHCLASS_SYMBOL_STR);
                break;

            case AbstractNodeType::CHCLASS_PUNCT:
            case AbstractNodeType::NCHCLASS_PUNCT:
                nodeRef.get()->GetDataRef().SetChSet(CHCLASS_PUNCT_STR);
                break;

            case AbstractNodeType::CHCLASS_CLOSURE:
            case AbstractNodeType::NCHCLASS_CLOSURE:
                nodeRef.get()->GetDataRef().SetChSet(CHCLASS_CLOSURE_STR);
                break;

            case AbstractNodeType::CHCLASS_MATH:
            case AbstractNodeType::NCHCLASS_MATH:
                nodeRef.get()->GetDataRef().SetChSet(CHCLASS_MATH_STR);
                break;

            default:
                break;
            }

            nodeRefVec.push_back(nodeRef);
            createNodeFlag = false;
        }
    }

    if (absNodeType == AbstractNodeType::LITERAL && !literalBuff.empty())
    {
        currId++;
        nodeRef = _createChClassNode(nodeRef, currId);
        nodeRef.get()->GetDataRef().SetChSet(literalBuff);
        literalBuff.clear();
    }

    if (chClassRangeFlag)
        throw RegenException::InvalidRangeException();

    if (!ctxStack.empty())
        throw RegenException::ContextMismatchException();

    _normalizeAST(nodeRefVec);

    return rootRef;
}