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

    if (*iterRef != '\\' || iterRef >= endIterRef)
        return TokenType::UNDEFINED;

    /// @brief Shorthand lambda for bound and dereference checking of a string iterator
    static auto iterPointsTo = [&](const std::string::iterator &iter, char ch)
    {
        return iter < endIterRef && *iter == ch;
    };

    short iterOff = 2;
    TokenType tokenType = TokenType::UNDEFINED;

    /// Detects non-compliant regex pre-defined character class constructs.
    if (!REGEN_REGEX_COMPLIANT_FLAG)
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

        //? CHCLASS_SYMBOL check placeholder
        case 's':
            if (iterPointsTo(iterRef + 2, 'b'))
            {
                tokenType = TokenType::CHCLASS_SYMBOL;
                iterOff++;
            }
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

        //? NCHCLASS_SYMBOL check placeholder
        case 'S':
            if (iterPointsTo(iterRef + 2, 'B'))
            {
                tokenType = TokenType::NCHCLASS_SYMBOL;
                iterOff++;
            }
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

    //? CHCLASS_SPACE check placeholder
    case 's':
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

    //? NCHCLASS_SPACE check placeholder
    case 'S':
        tokenType = TokenType::NCHCLASS_SPACE;
        break;

    default:
        return TokenType::UNDEFINED;
    }

    iterRef += iterOff;
    return tokenType;
}

RegenParser::TokenType RegenParser::Parser::_scanOperator(const std::string::iterator &beginIterRO, const std::string::iterator &endIterRO, std::string::iterator &iterRef, std::stack<RegenParser::ContextGroup> &ctxGrStackRef)
{
    short iterOff;
    TokenType tokenType;

    /// @brief Shorthand lambda for bound and dereference checking of a string iterator
    static auto iterPointsTo = [&](const std::string::iterator &iter, char ch)
    {
        return iter < endIterRO && *iter == ch;
    };

    /// @brief Shorthand lambda for iterOff and type assignments
    auto setOffNType = [&](short newIterOff, enum TokenType newType)
    {
        iterOff = newIterOff;
        tokenType = newType;
    };

    /// @brief Shorthand lambda for the stack context checking condition
    auto cmpStackTop = [](const std::stack<ContextGroup> &stack, enum ContextGroup ctxGr)
    {
        return !stack.empty() && stack.top() == ctxGr;
    };

    switch (*iterRef)
    {
    //? CHCLASS_BEGIN check placeholder
    case '[':
        if (iterPointsTo(iterRef + 1, '^'))
            setOffNType(2, TokenType::NCHCLASS_BEGIN);
        else
            setOffNType(1, TokenType::CHCLASS_BEGIN);
        if (!cmpStackTop(ctxGrStackRef, ContextGroup::CHCLASS))
            ctxGrStackRef.push(ContextGroup::CHCLASS);
        break;

    //? CHCLASS_END check placeholder
    case ']':
        if (!cmpStackTop(ctxGrStackRef, ContextGroup::CHCLASS))
            return TokenType::UNDEFINED;
        setOffNType(1, TokenType::CHCLASS_END);
        ctxGrStackRef.pop();
        break;

    //? CHCLASS_RANGE check placeholder
    case '-':
        if (!cmpStackTop(ctxGrStackRef, RegenParser::ContextGroup::CHCLASS))
            return TokenType::UNDEFINED;
        setOffNType(1, TokenType::CHCLASS_RANGE);
        break;

    //? CHCLASS_INT check placeholder
    case '&':
        if (!cmpStackTop(ctxGrStackRef, RegenParser::ContextGroup::CHCLASS) ||
            !REGEN_REGEX_COMPLIANT_FLAG ||
            !iterPointsTo(iterRef + 1, '&'))
            return TokenType::UNDEFINED;
        setOffNType(1, TokenType::CHCLASS_INT);
        break;

    default:
        return TokenType::UNDEFINED;
    }

    iterRef += iterOff;
    return tokenType;
}

RegenParser::TokenType RegenParser::Parser::_scanToken(const std::string::iterator &beginIterRO, std::string::iterator &endIterRO, std::string::iterator &iterRef, std::stack<ContextGroup> &ctxGrStackRef)
{
    if (iterRef >= endIterRO)
        return TokenType::UNDEFINED;

    if (TokenType tokenType; (tokenType = _scanDefinedChClass(iterRef, endIterRO)) != TokenType::UNDEFINED)
        return tokenType;

    return _scanOperator(beginIterRO, endIterRO, iterRef, ctxGrStackRef);
}

std::shared_ptr<RegenAST::ASTNode> RegenParser::Parser::_createLiteralNode(std::shared_ptr<RegenAST::ASTNode> nodeRef, int id, const std::string &str)
{
    auto childRef = std::make_shared<RegenAST::ASTNode>(id, RegenAST::NodeType::LITERAL, nodeRef);
    nodeRef->GetChildrenRef().emplace_back(childRef);
    childRef->GetDataRef().SetLiteral(str);

    return childRef;
}

std::shared_ptr<RegenAST::ASTNode> RegenParser::Parser::_createChClassNode(std::shared_ptr<RegenAST::ASTNode> nodeRef, int id)
{
    auto childRef = std::make_shared<RegenAST::ASTNode>(id, RegenAST::NodeType::CHCLASS, nodeRef);
    nodeRef->GetChildrenRef().emplace_back(childRef);

    return childRef;
}

void RegenParser::Parser::_normalizeAST(const std::vector<std::shared_ptr<RegenAST::ASTNode>> &nodeRefVec)
{
    // TODO: Implement transformations

    for (const auto &nodeRef : nodeRefVec)
    {
        /// Skips entry nodes. (safety check)
        if (RegenAST::NodeType type = nodeRef->GetDataRef().GetNodeType();
            type == RegenAST::NodeType::ENTRY)
            continue;

        /// Removes the redundant node.
        if (nodeRef->GetDataRef().Empty())
        {
            std::shared_ptr<RegenAST::ASTNode> parentRef = nodeRef->GetParentRef();

            parentRef->GetChildrenRef().remove(nodeRef);
            for (auto &childRef : nodeRef->GetChildrenRef())
            {
                childRef->SetParent(parentRef);
                parentRef->GetChildrenRef().emplace_back(childRef);
            }
        }
    }
}

void RegenParser::Parser::_setAbsNodeTypeNFlags(RegenParser::TokenType tokenType, RegenParser::AbstractNodeType &absNodeTypeRef, bool &createNodeFlagRef, bool &chClassRangeFlagRef, bool &chClassIntFlagRef)
{
    if (tokenType == TokenType::CHCLASS_END)
    {
        absNodeTypeRef = AbstractNodeType::CHCLASS;
        return;
    }

    if (tokenType == TokenType::CHCLASS_RANGE)
    {
        chClassRangeFlagRef = true;
        absNodeTypeRef = AbstractNodeType::CHCLASS;
        return;
    }

    if (tokenType == TokenType::CHCLASS_INT)
    {
        chClassIntFlagRef = true;
        absNodeTypeRef = AbstractNodeType::CHCLASS;
    }

    createNodeFlagRef = true;

    switch (tokenType)
    {
    case TokenType::CHCLASS_BEGIN:
        absNodeTypeRef = AbstractNodeType::CHCLASS;
        break;

    case TokenType::NCHCLASS_BEGIN:
        absNodeTypeRef = AbstractNodeType::NCHCLASS;
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
        absNodeTypeRef = AbstractNodeType::CHCLASS_UPPER;
        break;

    case TokenType::CHCLASS_ALPHA:
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

    case TokenType::NCHCLASS_ALPHA:
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

    default:
        createNodeFlagRef = false;
        absNodeTypeRef = AbstractNodeType::LITERAL;
        break;
    }
}

std::shared_ptr<RegenAST::ASTNode> RegenParser::Parser::ParseExpression(std::string &expressionRef)
{
    auto rootRef = std::make_shared<RegenAST::ASTNode>(0, RegenAST::NodeType::ENTRY, nullptr);

    if (expressionRef.empty())
        return rootRef;

    if (expressionRef.back() == '\\' && REGEN_REGEX_COMPLIANT_FLAG)
        throw RegenException::UnterminatedConstructException(ContextGroupToStr(ContextGroup::NONE));

    //? Parser flag declarations placeholder.
    bool createNodeFlag;
    bool chClassRangeFlag;
    bool chClassIntFlag;

    //? Range-related variable declaration placeholder.
    bool rangeStartChIsSet = false;
    bool rangeStopChIsSet = false;
    char rangeStartCh;
    char rangeStopCh;

    int currId = 0;

    std::string literalBuff;
    std::stack<RegenParser::ContextGroup> ctxGrStack;
    std::shared_ptr<RegenAST::ASTNode> nodeRef = rootRef;
    std::vector<std::shared_ptr<RegenAST::ASTNode>> nodeRefVec;

    //? The root (entry) node of the AST is circular. (root->Parent == root)
    rootRef->SetParent(rootRef);

    auto iter = expressionRef.begin();
    auto beginIter = expressionRef.begin();
    auto endIter = expressionRef.end();
    while (iter != expressionRef.end())
    {
        ContextGroup prevCtxGr = ctxGrStack.empty() ? ContextGroup::NONE : ctxGrStack.top();
        TokenType tokenType = RegenParser::Parser::_scanToken(beginIter, endIter, iter, ctxGrStack);

        if (tokenType == TokenType::UNDEFINED)
        {
            char escapeSeq;
            char resultCh = _scanEscape(iter, endIter, escapeSeq) ? escapeSeq : *iter;

            if (!chClassRangeFlag && !rangeStartChIsSet)
            {
                rangeStartChIsSet = true;
                rangeStartCh = resultCh;
            }
            else if (chClassRangeFlag && !rangeStopChIsSet)
            {
                rangeStopChIsSet = true;
                rangeStopCh = resultCh;
            }

            literalBuff += resultCh;
            iter++;
        }
        else
        {
            AbstractNodeType absNodeType;
            _setAbsNodeTypeNFlags(
                tokenType,
                absNodeType,
                createNodeFlag,
                chClassRangeFlag,
                chClassIntFlag);

            if (chClassRangeFlag &&
                (tokenType == TokenType::UNDEFINED ||
                 tokenType == TokenType::CHCLASS_BEGIN ||
                 tokenType == TokenType::NCHCLASS_BEGIN ||
                 tokenType == TokenType::CHCLASS_END))
            {
                short literalBuffOff;

                //? Misleading range check placeholder (#1)
                if (!rangeStartChIsSet && !rangeStopChIsSet)
                {
                    if (RegenOutput::OUTPUT_ENABLED)
                        RegenOutput::FMTPrintWarning(RegenOutput::WarningMessage::CHCLASS_MISLEADING_RANGE);
                    nodeRef->GetDataRef().SetChSet("-");
                    literalBuffOff = 0;
                }
                //? Misleading range check placeholder (#2)
                else if (!rangeStartChIsSet)
                {
                    if (RegenOutput::OUTPUT_ENABLED)
                        RegenOutput::FMTPrintWarning(RegenOutput::WarningMessage::CHCLASS_MISLEADING_RANGE);
                    nodeRef->GetDataRef().SetChSet("-");
                    if (rangeStopCh != '-')
                        nodeRef->GetDataRef().SetChSet(std::string(1, rangeStopCh));
                    literalBuffOff = 1;
                }
                //? Misleading range check placeholder (#3)
                else if (!rangeStopChIsSet)
                {
                    if (RegenOutput::OUTPUT_ENABLED)
                        RegenOutput::FMTPrintWarning(RegenOutput::WarningMessage::CHCLASS_MISLEADING_RANGE);
                    nodeRef->GetDataRef().SetChSet(std::string(1, rangeStartCh));
                    if (rangeStartCh != '-')
                        nodeRef->GetDataRef().SetChSet("-");
                    literalBuffOff = 1;
                }
                else
                {
                    nodeRef->GetDataRef().SetChSet(rangeStartCh, rangeStopCh);
                    literalBuffOff = 2;
                }

                if (literalBuffOff > 0)
                    literalBuff.erase(literalBuff.begin(), literalBuff.begin() + literalBuffOff);

                rangeStartChIsSet = false;
                rangeStopChIsSet = false;
                chClassRangeFlag = false;
            }

            if (nodeRef->GetId() > 0 &&
                (tokenType == TokenType::CHCLASS_BEGIN ||
                 tokenType == TokenType::NCHCLASS_BEGIN ||
                 tokenType == TokenType::CHCLASS_END))
            {
                nodeRef->GetDataRef().SetChSet(literalBuff);
                literalBuff.clear();

                if (tokenType == TokenType::CHCLASS_END &&
                    nodeRef->GetDataRef().Empty())
                {
                    if (RegenOutput::OUTPUT_ENABLED)
                        RegenOutput::FMTPrintWarning(RegenOutput::WarningMessage::CHCLASS_EMPTY);
                    if (nodeRef->GetId() > 0)
                    {
                        nodeRef = nodeRef->GetParentRef();
                        nodeRef->GetChildrenRef().pop_back();
                        nodeRefVec.pop_back();
                    }
                }
            }

            if (createNodeFlag)
            {
                if (prevCtxGr == ContextGroup::NONE && !literalBuff.empty())
                {
                    currId++;
                    nodeRef = _createLiteralNode(nodeRef, currId, literalBuff);
                    literalBuff.clear();
                }

                if (absNodeType < AbstractNodeType::GROUP)
                {
                    if (prevCtxGr != ContextGroup::CHCLASS)
                    {
                        currId++;
                        nodeRef = _createChClassNode(nodeRef, currId);
                    }

                    if (absNodeType >= AbstractNodeType::NCHCLASS)
                    {
                        if (!nodeRef->GetDataRef().Empty() || !literalBuff.empty())
                        {
                            RegenOutput::FMTPrintWarning(fmt::format(
                                RegenOutput::WarningMessage::CHCLASS_REDUNDANT_INIT,
                                !nodeRef->GetDataRef().Empty() ? nodeRef->GetDataRef().GetLiteral() : literalBuff));
                        }

                        nodeRef->GetDataRef().FillChSet(true);
                        nodeRef->GetDataRef().SetInvertFlag(true);
                    }
                }

                switch (absNodeType)
                {
                case AbstractNodeType::CHCLASS_DIGIT:
                case AbstractNodeType::NCHCLASS_DIGIT:
                    nodeRef->GetDataRef().SetChSet('0', '9');
                    break;

                case AbstractNodeType::CHCLASS_WORD:
                case AbstractNodeType::NCHCLASS_WORD:
                    nodeRef->GetDataRef().SetChSet('a', 'z');
                    nodeRef->GetDataRef().SetChSet('A', 'Z');
                    nodeRef->GetDataRef().SetChSet('0', '9');
                    nodeRef->GetDataRef().SetChSet("_");
                    break;

                case AbstractNodeType::CHCLASS_SPACE:
                case AbstractNodeType::NCHCLASS_SPACE:
                    nodeRef->GetDataRef().SetChSet(CHCLASS_SPACE_STR);
                    break;

                case AbstractNodeType::CHCLASS_LOWER:
                case AbstractNodeType::NCHCLASS_LOWER:
                    nodeRef->GetDataRef().SetChSet('a', 'z');
                    break;

                case AbstractNodeType::CHCLASS_UPPER:
                case AbstractNodeType::NCHCLASS_UPPER:
                    nodeRef->GetDataRef().SetChSet('A', 'Z');
                    break;

                case AbstractNodeType::CHCLASS_ALPHA:
                case AbstractNodeType::NCHCLASS_ALPHA:
                    nodeRef->GetDataRef().SetChSet('a', 'z');
                    nodeRef->GetDataRef().SetChSet('A', 'Z');
                    break;

                case AbstractNodeType::CHCLASS_ALNUM:
                case AbstractNodeType::NCHCLASS_ALNUM:
                    nodeRef->GetDataRef().SetChSet('a', 'z');
                    nodeRef->GetDataRef().SetChSet('A', 'Z');
                    nodeRef->GetDataRef().SetChSet('0', '9');
                    break;

                case AbstractNodeType::CHCLASS_SYMBOL:
                case AbstractNodeType::NCHCLASS_SYMBOL:
                    nodeRef->GetDataRef().SetChSet(CHCLASS_SYMBOL_STR);
                    break;

                case AbstractNodeType::CHCLASS_PUNCT:
                case AbstractNodeType::NCHCLASS_PUNCT:
                    nodeRef->GetDataRef().SetChSet(CHCLASS_PUNCT_STR);
                    break;

                case AbstractNodeType::CHCLASS_CLOSURE:
                case AbstractNodeType::NCHCLASS_CLOSURE:
                    nodeRef->GetDataRef().SetChSet(CHCLASS_CLOSURE_STR);
                    break;

                case AbstractNodeType::CHCLASS_MATH:
                case AbstractNodeType::NCHCLASS_MATH:
                    nodeRef->GetDataRef().SetChSet(CHCLASS_MATH_STR);
                    break;

                default:
                    break;
                }

                createNodeFlag = false;
                chClassRangeFlag = false;
                chClassIntFlag = false;
                rangeStartChIsSet = false;
                rangeStopChIsSet = false;

                if (!nodeRefVec.empty() && nodeRef != nodeRefVec.back())
                    nodeRefVec.push_back(nodeRef);
            }
        }
    }

    //? This additional check is for expressions that end in a literal.
    if (!literalBuff.empty())
    {
        currId++;
        _createLiteralNode(nodeRef, currId, literalBuff);
        literalBuff.clear();
    }

    if (!ctxGrStack.empty())
    {
        throw RegenException::UnterminatedConstructException(
            ContextGroupToStr(ctxGrStack.top()));
    }

    // TODO: Uncomment the method call
    _normalizeAST(nodeRefVec);

    return rootRef;
}