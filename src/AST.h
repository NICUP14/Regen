#include <list>
#include <bitset>
#include <memory>
#include <algorithm>
#include <functional>
#include "Exception.h"

#ifndef REGEN_AST_HEADER_H
#define REGEN_AST_HEADER_H

/// @brief  Encapsulates all the AST-related methods and custom data types.
namespace RegenAST
{
    const size_t CH_SET_SIZE = 255;

    /// @brief Defines all the possible types of nodes.
    enum class NodeType
    {
        ENTRY,
        LITERAL,
        CHCLASS,
    };

    /// @brief Represents a container used for storing information about a node.
    /// @brief The methods' behaviour changes based on the node type of the object.
    class ASTNodeData
    {
    public:
        /// @returns Returns whether the current object's data is empty.
        bool Empty() const;

        /// @return Returns the type of node of the current object.
        NodeType GetNodeType() const;

        /// @return Returns the string representation of the current object.
        std::string GetLiteral();

        /// @brief Sets the node type of the current object to the given type.
        void SetNodeType(NodeType type);

        /// @brief Sets the string representation the current literal node to the given string.
        void SetLiteral(const std::string &str);

        /// @brief Sets all the vales of the current character class node's set to the given boolean.
        void FillChSet(bool value = false);

        /// @brief Sets the set's value corresponding the given character to true.
        /// @param invertFlag The boolean parameter used to "invert" the function's behaviour.
        void SetChSet(char ch, bool invertFlag = false);

        /// @brief Sets the set's values corresponding the characters in the given string to true.
        /// @param invertFlag The boolean parameter used to "invert" the function's behaviour.
        void SetChSet(const std::string_view &strRO, bool invertFlag = false);

        /// @brief Sets the set's values corresponding the range defined by the two the given characters to true.
        /// @param invertFlag The boolean parameter used to "invert" the function's behaviour.
        void SetChSet(char startCh, char stopCh, bool invertFlag = false);

    private:
        NodeType _nodeType;
        std::bitset<CH_SET_SIZE> _chSet;
        std::string _literal;
    };

    /// @brief Represents a node in the AST of a parsed Regen expression.
    struct ASTNode
    {
    public:
        /// @brief The custom constructor of the ASTNode class.
        explicit ASTNode(int id, NodeType type, std::shared_ptr<ASTNode> parent) : _id(id), _parent(parent) { _nodeData.SetNodeType(type); }

        /// @brief The default constructor of the ASTNode class.
        ASTNode() = default;

        /// @brief A default destructor of the ASTNode class.
        ~ASTNode() = default;

        /// @return Returns a number relative to the order of the current object in the AST.
        int GetId() const;

        /// @return Returns a reference to the internal ASTNodeData container of th current object.
        ASTNodeData &GetDataRef();

        /// @return Returns a shared pointer to the parent of the curent object.
        std::shared_ptr<ASTNode> GetParentRef();

        /// @return Returns a reference to the internal std::list of shared pointers pointing to the children of the node
        std::list<std::shared_ptr<ASTNode>> &GetChildrenRef();

        /// @brief Sets the parent of the current object to the given shared pointer
        void SetParent(std::shared_ptr<ASTNode> parent);

    private:
        int _id;
        ASTNodeData _nodeData;
        std::shared_ptr<ASTNode> _parent;
        std::list<std::shared_ptr<ASTNode>> _children;
    };

};

#endif