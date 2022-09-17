#include "Parser.h"

// TODO: Remove nop
#define nop

inline char RegenParser::scanEscape(std::string::iterator &iter)
{
    if (*iter != '\\')
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

inline RegenParser::tokenType RegenParser::scanToken(std::string::iterator &iter, std::stack<contextType> &ctxStack)
{
    short iterOff;
    enum tokenType type;

    //? CHCLASS_DIGIT/CHCLASS_WORD/CHCLASS_SPACE handler placeholder
    if (*iter == '\\')
    {
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

inline RegenAST::ASTNode RegenParser::parseExpression(std::string &expression)
{
    // TODO: Should also handle the empty expression case

    std::string literalBuff(nullptr, expression.size());
    std::stack<RegenParser::contextType> ctxStack;
    RegenAST::ASTNode root{RegenAST::nodeType::ENTRY, root};
    RegenAST::ASTNode &nodeRef = root;

    char lastClassCh = '\0';
    bool ClassRangeFlag = false;

    auto iter = expression.begin();
    while (iter != expression.end())
    {
        tokenType type = RegenParser::scanToken(iter, ctxStack);

        if (type == tokenType::UNDEFINED)
        {
            if (ClassRangeFlag)
            {
                nodeRef.Data.SetChSet(lastClassCh, *iter);
                ClassRangeFlag = false;
            }

            literalBuff += *iter;
            iter++;
        }
        else
        {
            ClassRangeFlag = false;

            switch (type)
            {
            case tokenType::CHCLASS_BEGIN:
                nodeRef.Children.emplace_back(RegenAST::nodeType::CHCLASS, nodeRef);
                nodeRef = nodeRef.Children.back();
                break;

            case tokenType::CHCLASS_END:
                nodeRef = nodeRef.Parent;
                break;

            case tokenType::CHCLASS_RANGE:
                ClassRangeFlag = true;
                break;

            default:
                break;
            }

            // TODO: Also treat CHCLASS case
            if (!literalBuff.empty())
                break;

            if (!ctxStack.empty() && ctxStack.top() == contextType::CHCLASS)
            {
                nodeRef.Data.SetChSet(literalBuff);
                lastClassCh = literalBuff.back();
            }
            else
                nodeRef.Children.emplace_back(RegenAST::nodeType::LITERAL, nodeRef);
            literalBuff.clear();
        }
    }

    if (!ctxStack.empty())
        throw RegenException::ContextMismatchException();

    return root;
}