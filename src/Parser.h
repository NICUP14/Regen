#include <iostream>
#include <functional>
#include <string>
#include <stack>
#include "AST.h"
#include "Exception.h"

#ifndef REGEN_PARSER_HEADER_H
#define REGEN_PARSER_HEADER_H

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
    /// @param iter A modifiable iterator to a regen expression
    /// @return The character equivalent of the escape sequence
    //? The method below acts as an extension of the "scanToken" method char
    inline char scanEscape(std::string::iterator &iter);

    /// @brief Converts regen language constructs to their token counterpart
    /// @param iter A modifiable iterator to a regen expression
    /// @param ctxStack A stack used to keep count of nested context groups
    /// @return The character equivalent of the escape sequence
    inline tokenType scanToken(std::string::iterator &iter, std::stack<contextType> &ctxStack);

    inline RegenAST::ASTNode parseExpression(std::string &expression);

}

#endif