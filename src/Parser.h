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

/// @brief Encapsulates all the parser-related methods and custom data types.
namespace RegenParser
{
    /// @brief Defines all the possible pre-defined types of tokens of the regen language.
    enum class tokenType
    {
        UNDEFINED,
        GROUP_BEGIN,
        GROUP_END,
        GROUP_SEP,
        CHCLASS_WORD,
        CHCLASS_SPACE,
        CHCLASS_DIGIT,
        CHCLASS_AND,
        CHCLASS_RANGE,
        NCHCLASS_WORD,
        NCHCLASS_SPACE,
        NCHCLASS_DIGIT,
        NCHCLASS_BEGIN,
        CHCLASS_BEGIN,
        CHCLASS_END
    };

    /// @brief Defines all the possible context groups of the regen language.
    enum class contextGroup
    {
        CHCLASS,
        GROUP
    };

    /*
     * @brief Encapsulates all the methods used in for parsing a regen expression.
     * This class acts as the base of the parser.
     * Inherit this class to extend its functionality.
     */
    class Parser
    {
    public:
        /// @return Returns the AST representation of the given regen expression.
        static std::shared_ptr<RegenAST::ASTNode> ParseExpression(std::string &expression);

    protected:
        /// @return Returns the type of token corresponding to the regen language construct pointed by the given modifiable iterator.
        static tokenType _scanToken(std::string::iterator &iter, const std::string::iterator &endIter, std::stack<contextGroup> &ctxStack);

        /// @return Returns the character equivalent of the escape sequence pointed by the given modifiable iterator.
        static char _scanEscape(std::string::iterator &iter, const std::string::iterator &endIter);

        /// @param str The string to be stored in the NodeData container.
        /// @return A shared pointer to the newly created node of type literal.
        static std::shared_ptr<RegenAST::ASTNode> _createLiteralNode(std::shared_ptr<RegenAST::ASTNode> nodeRef, int id, std::string str);

        /// @return A shared pointer to the newly created node of type literal.
        static std::shared_ptr<RegenAST::ASTNode> _createChClassNode(std::shared_ptr<RegenAST::ASTNode> nodeRef, int id);

        /// @brief Removes or transforms redundant nodes based on the specifications of the regen language.
        /// @param nodeRefVec The vector used to store the references of the AST's nodes.
        static void _normalizeAST(const std::vector<std::shared_ptr<RegenAST::ASTNode>> &nodeRefVec);
    };
}

#endif