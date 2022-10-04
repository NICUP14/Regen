#include <Regen/Parser.h>

bool RegenParser::Parser::_scanEscape(std::string::const_iterator &iterRef, const std::string::const_iterator &endIter, char &escapeSeqRef)
{
    if (iterRef >= endIter)
        return false;

    if (*iterRef != '\\' || iterRef + 1 >= endIter)
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

    //? Carriage return escape sequence placeholder.
    case 'r':
        escapeSeqRef = '\r';
        break;

    default:
        //? Escapes the pre-defined Regen operators.
        escapeSeqRef = *iterRef;
    }

    return true;
}

RegenParser::TokenType RegenParser::Parser::_scanDefinedChClass(std::string::const_iterator &iterRef, const std::string::const_iterator &endIter)
{
    if (iterRef >= endIter)
        return TokenType::UNDEFINED;

    if (*iterRef != '\\' || iterRef >= endIter)
        return TokenType::UNDEFINED;

    /// @brief Shorthand lambda for bounds and dereference checking of a string iterator
    static auto iterPointsTo = [&](const std::string::const_iterator &iter, char ch)
    {
        return iter < endIter && *iter == ch;
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

RegenParser::TokenType RegenParser::Parser::_scanOperator(std::string::const_iterator &iterRef, const std::string::const_iterator &endIter, std::stack<ContextGroup> &ctxGrStackRef)
{
    short iterOff;
    TokenType tokenType;

    /// @brief Shorthand lambda for bound and dereference checking of a string iterator
    static auto iterPointsTo = [&](const std::string::const_iterator &otherIterRef, char ch)
    {
        return otherIterRef < endIter && *otherIterRef == ch;
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
        if (!cmpStackTop(ctxGrStackRef, ContextGroup::CHCLASS))
            return TokenType::UNDEFINED;
        setOffNType(1, TokenType::CHCLASS_RANGE);
        break;

    //? CHCLASS_INT check placeholder
    case '&':
        if (!cmpStackTop(ctxGrStackRef, ContextGroup::CHCLASS) ||
            !iterPointsTo(iterRef + 1, '&') ||
            REGEN_REGEX_COMPLIANT_FLAG)
            return TokenType::UNDEFINED;
        setOffNType(2, TokenType::CHCLASS_INT);
        break;

    default:
        return TokenType::UNDEFINED;
    }

    iterRef += iterOff;
    return tokenType;
}

RegenParser::TokenType RegenParser::Parser::_scanToken(std::string::const_iterator &iterRef, const std::string::const_iterator &endIter, std::stack<ContextGroup> &ctxGrStackRef)
{
    if (iterRef >= endIter)
        return TokenType::UNDEFINED;

    if (TokenType tokenType; (tokenType = _scanDefinedChClass(iterRef, endIter)) != TokenType::UNDEFINED)
        return tokenType;

    return _scanOperator(iterRef, endIter, ctxGrStackRef);
}

void RegenParser::Parser::_setAbsNodeTypeNFlags(TokenType tokenType, ContextGroup prevCtxGr, AbstractNodeType &absNodeTypeRef, bool &createNodeFlagRef, bool &chClassRangeFlagRef, bool &chClassIntersectFlagRef)
{
    if (tokenType == TokenType::CHCLASS_RANGE)
    {
        chClassRangeFlagRef = true;
        absNodeTypeRef = AbstractNodeType::UNDEFINED;

        return;
    }

    if (tokenType == TokenType::CHCLASS_INT)
    {
        chClassIntersectFlagRef = true;
        absNodeTypeRef = AbstractNodeType::UNDEFINED;

        return;
    }

    //? LITERAL check placeholder.
    if (tokenType == TokenType::UNDEFINED)
    {
        createNodeFlagRef = prevCtxGr == ContextGroup::NONE;
        absNodeTypeRef = AbstractNodeType::LITERAL;

        return;
    }

    createNodeFlagRef = prevCtxGr != ContextGroup::CHCLASS;

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
        absNodeTypeRef = AbstractNodeType::UNDEFINED;
        break;
    }
}

std::shared_ptr<RegenAST::ASTNode> RegenParser::Parser::_createNode(AbstractNodeType absNodeType, int &id, std::shared_ptr<RegenAST::ASTNode> parentPtr)
{
    /// @brief Shorthand lambda for returning a pointer to a newly allocated node.
    static auto allocNode = [](int nodeId, RegenAST::NodeType nodeType, std::shared_ptr<RegenAST::ASTNode> nodeParentPtr)
    {
        auto childRef = std::make_shared<RegenAST::ASTNode>(nodeId, nodeType, nodeParentPtr);
        nodeParentPtr->GetChildrenRef().emplace_back(childRef);

        return childRef;
    };

    /// Creates character class nodes when needed.
    if (absNodeType == AbstractNodeType::LITERAL &&
        parentPtr->GetNodeDataPtr()->GetNodeType() != RegenAST::NodeType::LITERAL)
    {
        id++;
        return allocNode(id, RegenAST::NodeType::LITERAL, parentPtr);
    }

    /// Creates literal nodes when needed.
    if (absNodeType >= AbstractNodeType::CHCLASS &&
        absNodeType < AbstractNodeType::GROUP)
    {
        id++;
        return allocNode(id, RegenAST::NodeType::CHCLASS, parentPtr);
    }

    return nullptr;
}

void RegenParser::Parser::_initNode(std::shared_ptr<RegenAST::ASTNode> nodePtr, AbstractNodeType absNodeType)
{
    auto &chClassDataRef = RegenAST::CastToChClassNodeData(*nodePtr->GetNodeDataPtr());

    /// Performs the default negated character class initialization procedure.
    if (absNodeType >= AbstractNodeType::NCHCLASS)
    {
        if (!chClassDataRef.Empty())
            RegenOutput::FMTPrintWarning(fmt::format(
                RegenOutput::WarningMessage::CHCLASS_REDUNDANT_INIT,
                chClassDataRef.GetLiteral()));

        chClassDataRef.SetInvertFlag(true);
        chClassDataRef.Reset();
    }

    /// Performs the pre-defined character class initialization procedure.
    switch (absNodeType)
    {
    case AbstractNodeType::CHCLASS_DIGIT:
    case AbstractNodeType::NCHCLASS_DIGIT:
        chClassDataRef.Set('0', '9');
        break;

    case AbstractNodeType::CHCLASS_WORD:
    case AbstractNodeType::NCHCLASS_WORD:
        chClassDataRef.Set('a', 'z');
        chClassDataRef.Set('A', 'Z');
        chClassDataRef.Set('0', '9');
        chClassDataRef.Set("_");
        break;

    case AbstractNodeType::CHCLASS_SPACE:
    case AbstractNodeType::NCHCLASS_SPACE:
        chClassDataRef.Set(CHCLASS_SPACE_INIT);
        break;

    case AbstractNodeType::CHCLASS_LOWER:
    case AbstractNodeType::NCHCLASS_LOWER:
        chClassDataRef.Set('a', 'z');
        break;

    case AbstractNodeType::CHCLASS_UPPER:
    case AbstractNodeType::NCHCLASS_UPPER:
        chClassDataRef.Set('A', 'Z');
        break;

    case AbstractNodeType::CHCLASS_ALPHA:
    case AbstractNodeType::NCHCLASS_ALPHA:
        chClassDataRef.Set('a', 'z');
        chClassDataRef.Set('A', 'Z');
        break;

    case AbstractNodeType::CHCLASS_ALNUM:
    case AbstractNodeType::NCHCLASS_ALNUM:
        chClassDataRef.Set('a', 'z');
        chClassDataRef.Set('A', 'Z');
        chClassDataRef.Set('0', '9');
        break;

    case AbstractNodeType::CHCLASS_SYMBOL:
    case AbstractNodeType::NCHCLASS_SYMBOL:
        chClassDataRef.Set(CHCLASS_SYMBOL_INIT);
        break;

    case AbstractNodeType::CHCLASS_PUNCT:
    case AbstractNodeType::NCHCLASS_PUNCT:
        chClassDataRef.Set(CHCLASS_PUNCT_INIT);
        break;

    case AbstractNodeType::CHCLASS_CLOSURE:
    case AbstractNodeType::NCHCLASS_CLOSURE:
        chClassDataRef.Set(CHCLASS_CLOSURE_INIT);
        break;

    case AbstractNodeType::CHCLASS_MATH:
    case AbstractNodeType::NCHCLASS_MATH:
        chClassDataRef.Set(CHCLASS_MATH_INIT);
        break;

    default:
        break;
    }
}

void RegenParser::Parser::_initNode(std::shared_ptr<RegenAST::ASTNode> nodePtr, bool rangeStartChIsSet, bool rangeStopChIsSet, char rangeStartCh, char rangeStopCh)
{

    const auto &chClassDataRef = RegenAST::CastToChClassNodeData(*nodePtr->GetNodeDataPtr());

    //? Misleading range check placeholder (#1)
    if (!rangeStartChIsSet && !rangeStopChIsSet)
    {
        RegenOutput::FMTPrintWarning(RegenOutput::WarningMessage::CHCLASS_MISLEADING_RANGE);
        chClassDataRef.Set("-");
    }
    //? Misleading range check placeholder (#2)
    else if (!rangeStartChIsSet)
    {
        RegenOutput::FMTPrintWarning(RegenOutput::WarningMessage::CHCLASS_MISLEADING_RANGE);
        chClassDataRef.Set("-");
        if (rangeStopCh != '-')
        {
            chClassDataRef.Reset(rangeStopCh);
            chClassDataRef.Set((std::string) "" + rangeStopCh);
        }
    }
    //? Misleading range check placeholder (#3)
    else if (!rangeStopChIsSet)
    {
        RegenOutput::FMTPrintWarning(RegenOutput::WarningMessage::CHCLASS_MISLEADING_RANGE);
        chClassDataRef.Set("-");
        if (rangeStartCh != '-')
        {
            chClassDataRef.Reset(rangeStartCh);
            chClassDataRef.Set((std::string) "" + rangeStartCh);
        }
    }
    else
    {
        if (rangeStartCh > rangeStopCh)
        {
            if (REGEN_REGEX_COMPLIANT_FLAG)
                throw RegenException::InvalidRegexRangeException();

            std::swap(rangeStartCh, rangeStopCh);
        }

        /// Performs character class range initialization procedure.
        chClassDataRef.Reset(rangeStartCh);
        chClassDataRef.Reset(rangeStopCh);
        chClassDataRef.Set(rangeStartCh, rangeStopCh);
    }
}

void RegenParser::Parser::_normalizeAST(std::vector<std::shared_ptr<RegenAST::ASTNode>> &nodePtrVec)
{
    /// @brief Shorthand lambda for completely removing a node from the AST.
    static auto removeNodeFromAST = [](std::shared_ptr<RegenAST::ASTNode> nodePtr)
    {
        auto parentPtr = nodePtr->GetParentPtr();

        parentPtr->GetChildrenRef().remove(nodePtr);
        for (const auto &childRef : nodePtr->GetChildrenRef())
            parentPtr->GetChildrenRef().emplace_back(childRef);
    };

    auto iter = nodePtrVec.begin();
    while (iter < nodePtrVec.end())
    {
        /// Performs the character class normalization procedure.
        if (auto nodeDataRef = (*iter)->GetNodeDataPtr();
            nodeDataRef->GetNodeType() == RegenAST::NodeType::CHCLASS)
        {
            RegenAST::CastToChClassNodeData(*nodeDataRef).Normalize();

            if (nodeDataRef->Empty())
            {
                /// Removes the empty node from the ast
                RegenOutput::FMTPrintWarning(RegenOutput::WarningMessage::CHCLASS_EMPTY);
                removeNodeFromAST(*iter);

                /// Performs an in-place removal of the vector element.
                iter++;
                nodePtrVec.erase(iter - 1);

                continue;
            }
        }

        iter++;
    }
}

std::shared_ptr<RegenAST::ASTNode> RegenParser::Parser::ParseExpression(const std::string &expression)
{
    auto rootPtr = std::make_shared<RegenAST::ASTNode>(0, RegenAST::NodeType::ENTRY, nullptr);

    /// The root node is defined in a circular manner.
    rootPtr->SetParent(rootPtr);

    if (expression.empty())
        return rootPtr;

    /// Checks for a trailing backslash in the given Regen expression.
    if (expression.back() == '\\' && REGEN_REGEX_COMPLIANT_FLAG)
        throw RegenException::UnterminatedConstructException();

    //? Parser flag declarations placeholder.
    bool createNodeFlag = false;
    bool chClassRangeFlag = false;
    bool chClassIntersectFlag = false;

    //? Range-related auxiliary variable declarations placeholder.
    bool rangeStartChIsSet = false;
    bool rangeStopChIsSet = false;
    char rangeStartCh;
    char rangeStopCh;

    //? AST-related auxiliary variable declarations placeholder.
    int currId = 0;
    std::stack<ContextGroup> ctxGrStack;
    std::shared_ptr<RegenAST::ASTNode> nodePtr = rootPtr;
    std::vector<std::shared_ptr<RegenAST::ASTNode>> nodePtrVec;

    auto iter = expression.begin();
    auto endIter = expression.end();
    while (iter != expression.end())
    {
        ContextGroup prevCtxGr = ctxGrStack.empty() ? ContextGroup::NONE : ctxGrStack.top();
        TokenType tokenType = _scanToken(iter, endIter, ctxGrStack);
        AbstractNodeType absNodeType;

        //? Flag and abstract Node type setter procedure placeholder.
        _setAbsNodeTypeNFlags(
            tokenType,
            prevCtxGr,
            absNodeType,
            createNodeFlag,
            chClassRangeFlag,
            chClassIntersectFlag);

        //? Node creation procedure placeholder.
        if (createNodeFlag)
        {
            if (auto newNodeRef = _createNode(absNodeType, currId, nodePtr); newNodeRef != nullptr)
                nodePtr = newNodeRef;

            if (nodePtrVec.empty() || nodePtr != nodePtrVec.back())
                nodePtrVec.push_back(nodePtr);

            createNodeFlag = false;
        }

        //? Pre-defined initialization procedure placeholder.
        if (absNodeType >= AbstractNodeType::CHCLASS &&
            absNodeType < AbstractNodeType::GROUP)
            _initNode(nodePtr, absNodeType);

        //? Intersect initialization procedure placeholder. (lazy initialization)
        if (chClassIntersectFlag)
        {
            auto &chClassDataRef = RegenAST::CastToChClassNodeData(*nodePtr->GetNodeDataPtr());

            chClassDataRef.SetIntersectFlag(true);
            chClassDataRef.SetInvertFlag(false);

            chClassIntersectFlag = false;
        }

        //? Literal and custom character class initialization procedure placeholder.
        if (absNodeType == AbstractNodeType::LITERAL)
        {
            char escapeSeq;
            char resultCh = _scanEscape(iter, endIter, escapeSeq) ? escapeSeq : *iter;

            /// Updates the range-related variables.
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

            nodePtr->GetNodeDataPtr()->Append(resultCh);
            iter++;

            //? Range initialization procedure placeholder.
            if (chClassRangeFlag)
            {
                _initNode(
                    nodePtr,
                    rangeStartChIsSet,
                    rangeStopChIsSet,
                    rangeStartCh,
                    rangeStopCh);

                rangeStartChIsSet = false;
                rangeStopChIsSet = false;
                chClassRangeFlag = false;
            }
        }
    }

    if (!ctxGrStack.empty())
        throw RegenException::UnterminatedConstructException();

    //? AST normalization procedure placeholder.
    _normalizeAST(nodePtrVec);

    return rootPtr;
}