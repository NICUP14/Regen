#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <stack>
#include <memory>
#include "AST.h"
#include "Exception.h"
#include "Output.h"

#ifndef REGEN_PARSER_HEADER_H
#define REGEN_PARSER_HEADER_H

extern const bool REGEN_REGEX_COMPLIANT_FLAG;

/// @brief Encapsulates all the parser-related methods and custom data types.
namespace RegenParser
{
    const std::string_view CHCLASS_SPACE_INIT = "\0\n\t";
    const std::string_view CHCLASS_SYMBOL_INIT = "!@#$%^&*?";
    const std::string_view CHCLASS_PUNCT_INIT = "\t;`:' ,\".?_";
    const std::string_view CHCLASS_CLOSURE_INIT = "(){}[]";
    const std::string_view CHCLASS_MATH_INIT = "+-*/%=<>";

    /// @brief Defines all the possible pre-defined types of tokens of the Regen language.
    enum class TokenType
    {
        UNDEFINED,
        CHCLASS_BEGIN,
        CHCLASS_END, //? CHCLASS_END also incorporates the token of type NCHCLASS_END.
        CHCLASS_INT,
        CHCLASS_RANGE,
        CHCLASS_WORD,
        CHCLASS_SPACE,
        CHCLASS_DIGIT,
        CHCLASS_LOWER,
        CHCLASS_UPPER,
        CHCLASS_ALPHA,
        CHCLASS_ALNUM,
        CHCLASS_SYMBOL,
        CHCLASS_PUNCT,
        CHCLASS_CLOSURE,
        CHCLASS_MATH,
        NCHCLASS_BEGIN,
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
        GROUP_BEGIN,
        GROUP_END,
        GROUP_SEP,
    };

    /// @brief Defines all the possible context groups of the Regen language.
    enum class ContextGroup
    {
        NONE,
        CHCLASS,
        GROUP
    };

    /// @brief Defines all the possible abstract nodes types of the Regen language.
    /// @brief Abstract node types will be converted to Regen-compatible ones during parsing.
    /// WARNING: NCHCLASS-related node type values must always be greater than the CHCLASS-related ones.
    enum class AbstractNodeType
    {
        UNDEFINED,
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
        GROUP,
    };

    /// @brief Encapsulates all the methods used in for parsing a Regen expression.
    /// @brief This class acts as the base of the parser.
    /// @brief Inherit this class to extend its functionality.
    class Parser
    {
    public:
        /// @brief Represents a char-by-char parser.
        /// @return Returns the AST representation of the given Regen expression.
        static std::shared_ptr<RegenAST::ASTNode> ParseExpression(std::string &expressionRef);

    protected:
        /// @brief Checks if there exists an escape sequence pointed by the given modifiable iterator and stores the character representation of the sequence in escapeSeqRef.
        /// @return Returns true if there exists a character equivalent of the escape sequence pointed by the given modifiable iterator, otherwise false.
        static bool _scanEscape(std::string::iterator &iterRef, const std::string::iterator &endIterRef, char &escapeSeqRef);

        /// @return Returns the type of token corresponding to the pre-defined character class constructs of the Regen language pointed by the given modifiable operator.
        /// @return The method supports both regex compliant and non-compliant pre-defined character class constructs.
        static TokenType _scanDefinedChClass(std::string::iterator &iterRef, const std::string::iterator &endIterRef);

        /// @return Returns the type of token corresponding to the operator construct of the Regen language pointed by the given modifiable iterator.
        static TokenType _scanOperator(std::string::iterator &iterRef, const std::string::iterator &endIterRef, std::stack<ContextGroup> &ctxGrStackRef);

        /// @brief The method acts as a wrapper incorporating all scanning-related functions. (except the _scanEscape method)
        /// @return Returns the type of token corresponding to the Regen language construct pointed by the given modifiable iterator.
        static TokenType _scanToken(std::string::iterator &iterRef, const std::string::iterator &endIterRef, std::stack<ContextGroup> &ctxGrStackRef);

        /// @brief Converts the given token type to its corresponding abstract node type. (assignment might be skipped)
        /// @brief Additionally sets the createNode, chRangeFlag, chClassIntersectFlag flags in accordance with the determined abstract node type.
        static void _setAbsNodeTypeNFlags(TokenType tokenType, ContextGroup prevCtxGr, AbstractNodeType &absNodeTypeRef, bool &createNodeFlagRef, bool &chClassRangeFlagRef, bool &chClassIntersectFlagRef);

        /// @brief Performs duplicate node checking and creates a new node when needed.
        /// WARNING: The method always returns nullptr on a failed duplicate node checks.
        /// @return Returns a shared pointer to a newly allocated if a new node is needed.
        static std::shared_ptr<RegenAST::ASTNode> _createNode(AbstractNodeType absNodeType, int &id, std::shared_ptr<RegenAST::ASTNode> parentRef);

        /// @brief Initializes the NodeData container of a node in accordance with the given abstract node type. (acts as the pre-defined character class initializer)
        static void _initNode(std::shared_ptr<RegenAST::ASTNode> nodeRef, AbstractNodeType absNodeType);

        /// @brief Performs fake-range checking and initializes the NodeData container of a node to the given range. (acts both as the range custom character class initializer and custom initializer)
        static void _initNode(std::shared_ptr<RegenAST::ASTNode> nodeRef, bool rangeStartChIsSet, bool rangeStopChIsSet, char rangeStartCh, char rangeStopCh);

        /// @brief Normalizes the AST's nodes. The method removes empty nodes and performs character class normalizations.
        static void _normalizeAST(std::vector<std::shared_ptr<RegenAST::ASTNode>> &nodeRefVec);
    };
}

#endif