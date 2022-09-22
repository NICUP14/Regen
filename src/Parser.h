#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <stack>
#include <memory>
#include "AST.h"
#include "Exception.h"
#include "Output.h"

// const bool REGEN_REGEX_COMPLIANT;

#ifndef REGEN_PARSER_HEADER_H
#define REGEN_PARSER_HEADER_H

/// @brief Encapsulates all the parser-related methods and custom data types.
namespace RegenParser
{
    const std::string_view CHCLASS_SPACE_STR = "\0\n\t";
    const std::string_view CHCLASS_SYMBOL_STR = "!@#$%^&*?";
    const std::string_view CHCLASS_PUNCT_STR = "\t;`:' ,\".?_";
    const std::string_view CHCLASS_CLOSURE_STR = "(){}[]";
    const std::string_view CHCLASS_MATH_STR = "+-*/%=<>";

    /// @brief Defines all the possible pre-defined types of tokens of the regen language.
    enum class TokenType
    {
        UNDEFINED,
        GROUP_BEGIN,
        GROUP_END,
        GROUP_SEP,
        CHCLASS_WORD,
        CHCLASS_SPACE,
        CHCLASS_BEGIN,
        CHCLASS_END,
        CHCLASS_AND,
        CHCLASS_RANGE,
        CHCLASS_DIGIT,
        CHCLASS_LOWER,
        CHCLASS_UPPER,
        CHCLASS_ALPHA,
        CHCLASS_ALNUM,
        CHCLASS_SYMBOL,
        CHCLASS_PUNCT,
        CHCLASS_CLOSURE,
        CHCLASS_MATH,
        NCHCLASS_BEGIN, //? The token of type CHCLASS_END incorporates NCHCLASS_END.
        NCHCLASS_DIGIT,
        NCHCLASS_WORD,
        NCHCLASS_SPACE,
        NCHCLASS_LOWER,
        NCHCLASS_UPPER,
        NCHCLASS_ALPHA,
        NCHCLASS_ALNUM,
        NCHCLASS_SYMBOL,
        NCHCLASS_PUNCT,
        NCHCLASS_CLOSURE,
        NCHCLASS_MATH,
    };

    /// @brief Defines all the possible context groups of the regen language.
    enum class ContextGroup
    {
        CHCLASS,
        GROUP
    };

    // TODO: Find the right place in the source files to put this comment.
    //? A negated character class construct will be internally converted to a character class.

    /// @brief Defines all the possible nodes types of the regen language. (except the entry node type)
    /// WARNING: NCHCLASS-related node type values must be greater than the CHCLASS-related ones.
    enum class AbstractNodeType
    {
        LITERAL,
        CHCLASS,
        CHCLASS_DIGIT,
        CHCLASS_WORD,
        CHCLASS_SPACE,
        CHCLASS_LOWER,
        CHCLASS_UPPER,
        CHCLASS_ALPHA,
        CHCLASS_ALNUM,
        CHCLASS_SYMBOL,
        CHCLASS_PUNCT,
        CHCLASS_CLOSURE,
        CHCLASS_MATH,
        NCHCLASS,
        NCHCLASS_DIGIT,
        NCHCLASS_WORD,
        NCHCLASS_SPACE,
        NCHCLASS_LOWER,
        NCHCLASS_UPPER,
        NCHCLASS_ALPHA,
        NCHCLASS_ALNUM,
        NCHCLASS_SYMBOL,
        NCHCLASS_PUNCT,
        NCHCLASS_CLOSURE,
        NCHCLASS_MATH,
    };

    /*
     * @brief Encapsulates all the methods used in for parsing a regen expression.
     * This class acts as the base of the parser.
     * Inherit this class to extend its functionality.
     */
    class Parser
    {
    public:
        /// @brief Represents a char-by-char parser.
        /// @return Returns the AST representation of the given regen expression.
        static std::shared_ptr<RegenAST::ASTNode> ParseExpression(std::string &expressionRef);

    protected:
        /// @return Returns true if there exists a character equivalent of the escape sequence pointed by the given modifiable iterator, otherwise false.
        static bool _scanEscape(std::string::iterator &iterRef, const std::string::iterator &endIterRef, char &escapeSeqRef);

        /*
         * @return Returns the type of token corresponding to the pre-defined character class constructs of the regen language pointed by the given modifiable operator.
         * The method supports both regex compliant and non-compliant pre-defined character class constructs.
         */
        static TokenType _scanDefinedChClass(std::string::iterator &iterRef, const std::string::iterator &endIterRef);

        /// @return Returns the type of token corresponding to the operator construct of the regen language pointed by the given modifiable iterator.
        static TokenType _scanOperator(std::string::iterator &iterRef, const std::string::iterator &endIterRef, std::stack<ContextGroup> &ctxGrStackRef);

        /// @brief The method acts as a wrapper incorporating all functions that whose name begin with scan.
        /// @return Returns the type of token corresponding to the regen language construct pointed by the given modifiable iterator.
        static TokenType _scanToken(std::string::iterator &iterRef, const std::string::iterator &endIterRef, std::stack<ContextGroup> &ctxGrStackRef);

        /*
         * @brief Converts the given token type to its corresponding abstract node type. (assignment might be skipped)
         * Additionally sets the createNode and chRangeFlags in compliance with the abstract node type
         */
        static void _setAbsNodeTypeNFlags(TokenType tokenType, AbstractNodeType &absNodeTypeRef, bool &createNodeFlagRef, bool &chClassRangeFlagRef);

        /// @param str The string to be stored in the NodeData container.
        /// @return A shared pointer to the newly created node of type literal.
        static std::shared_ptr<RegenAST::ASTNode> _createLiteralNode(std::shared_ptr<RegenAST::ASTNode> nodeRef, int id, const std::string &str);

        /// @return A shared pointer to the newly created node of type literal.
        static std::shared_ptr<RegenAST::ASTNode> _createChClassNode(std::shared_ptr<RegenAST::ASTNode> nodeRef, int id);

        /// @brief Removes or transforms redundant nodes based on the specifications of the regen language.
        /// @param nodeRefVec The vector used to store the references of the AST's nodes.
        static void _normalizeAST(const std::vector<std::shared_ptr<RegenAST::ASTNode>> &nodeRefVec);
    };
}

#endif