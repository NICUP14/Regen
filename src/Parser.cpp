#include <iostream>
#include <stack>

#include "Exception.h"

namespace RegenParser
{
    /// @brief Lists all the possible token types of the regen language
    enum class tokenType
    {
        UNDEFINED,
        GROUP_BEGIN,
        GROUP_END,
        GROUP_SEP,
        CHCLASS_WORD,
        CHCLASS_SPACE,
        CHCLASS_DIGIT,
        CHCLASS_RANGE,
        CHCLASS_BEGIN,
        CHCLASS_END
    };

    /// @brief Lists all the possible token types of the regen language
    //? expression constructs can take different token types based on the current context
    enum class contextType
    {
        CHCLASS,
        GROUP
    };

    /// @brief Detects escape sequences defined the regen language
    /// @param strIter An modifiable iterator to a regen expression
    /// @return The character equivalent of the escape sequence
    //? The method below acts as an extension of the "scanToken" method
    char
    scanEscape(std::string::iterator &iter)
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

    enum tokenType scanToken(std::string::iterator &iter, std::stack<contextType> &ctxStack)
    {
        short iterOff;
        enum tokenType type;

        /// Detects default character class ids
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
                type = tokenType::LITERAL;
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
            return stack.empty() || stack.top() != ctx;
        };

        /// Detects custom character classes
        if (*iter == '[')
        {
            ctxStack.push(contextType::CHCLASS);

            setOffNType(1, tokenType::CHCLASS_BEGIN);
            return type;
        }

        if (*iter == ']')
        {
            if (cmpStackTop(ctxStack, contextType::CHCLASS))
                throw RegenException::contextMismatch();

            ctxStack.pop();

            setOffNType(1, tokenType::CHCLASS_END);
            return type;
        }

        /// Detects groups
        if ((*iter == '(' || *iter == ')') &&
            cmpStackTop(ctxStack, contextType::CHCLASS))
        {
            setOffNType(0, tokenType::UNDEFINED);
            return type;
        }

        if (*iter == '(')
        {
            ctxStack.push(contextType::GROUP);

            setOffNType(1, tokenType::GROUP_BEGIN);
            return type;
        }

        if (*iter == ')')
        {
            if (cmpStackTop(ctxStack, contextType::GROUP))
                throw RegenException::ContextMismatchException();

            setOffNType(1, tokenType::GROUP_END);
            return type;
        }

        /// Detects character class-specific tokens
        if (*iter == '-' &&
            cmpStackTop(ctxStack, tokenType::CHCLASS))
        {
            setOffNType(1, tokenType::CHCLASS_RANGE);
            return type;
        }

        /// Detects group-specific tokens
        if (*iter == '|' &&
            cmpStackTop(ctxStack, contextType::GROUP))
        {
            setOffNType(1, tokenType::GROUP_SEP);
            return type;
        }

        if (!ctxStack.empty())
            throw RegenException::ContextMismatchException();

        setOffNType(0, tokenType::UNDEFINED);
        return type;
    }
}
}