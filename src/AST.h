#include <list>
#include <bitset>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>
#include "Exception.h"

#ifndef REGEN_AST_HEADER_H
#define REGEN_AST_HEADER_H

extern const bool REGEN_REGEX_COMPLIANT_FLAG;

/// @brief  Encapsulates all the AST-related custom data types.
namespace RegenAST
{
    /// @brief Defines the fixed size of a character set.
    const size_t CH_SET_SIZE = 255;

    /// @brief Defines all the possible types of nodes.
    enum class NodeType
    {
        ENTRY,
        LITERAL,
        CHCLASS,
    };

    /// @brief Represents a base container used for storing information about a node.
    class ASTNodeData
    {
    public:
        virtual ~ASTNodeData() = default;

        /// @return Returns the type of node of the current object.
        NodeType GetNodeType() const;

        /// @returns Returns whether the current object's data is empty.
        virtual bool Empty() const;

        /// @return Returns the string representation of the current object.
        virtual std::string GetLiteral();

        /// @brief Sets the node type of the current object to the given type.
        void SetNodeType(NodeType nodeType);

        /// @brief Appends the given character to the current object's container.
        virtual void Append(char ch);

    private:
        NodeType _nodeType;
        std::string _literal;

    protected:
        /// @return Returns the string representation of the current object. (bypasses the overriden GetLiteral method)
        std::string _getLiteral() const;

        /// @brief Sets the given string as the new container of the current object.
        void _setLiteral(const std::string_view &literalRO);
    };

    class ASTChClassNodeData : public ASTNodeData
    {
    public:
        ASTChClassNodeData() { SetNodeType(NodeType::CHCLASS); };

        ~ASTChClassNodeData() override = default;

        /// @returns Returns whether the current object's data is empty.
        bool Empty() const override;

        /// @return Returns the string representation of the current object.
        std::string GetLiteral() override;

        /// @brief Doesn't do anything. (overrides the base method for the added safety)
        void Append(char ch) override;

        /// @brief Sets the character set invert flag to the specified value.
        void SetInvertFlag(bool value);

        /// @brief Sets the character set intersect flag to the specified value.
        void SetIntersectFlag(bool value);

        /// @brief Sets all the vales of the set in accordance with the invert flag.
        void Reset();

        /// @brief Sets the set's values corresponding the given character in accordance with the invert flag.
        void Set(char ch);

        /// @brief Sets the set's values corresponding the characters in the given string in accordance with the invert flag.
        void Set(const std::string_view &strRO);

        /// @brief Sets the set's values corresponding the characters in the specified range in accordance with the invert flag.
        void Set(char startCh, char stopCh);

        /// @brief Merges the two character sets and resets the flags.
        void Normalize();

    private:
        std::bitset<CH_SET_SIZE> _primaryChSet;
        std::bitset<CH_SET_SIZE> _auxiliaryChSet;
        bool _chSetInvertFlag = false;
        bool _chSetIntersectFlag = false;
    };

    /// @brief Represents a node of the Regen AST.
    class ASTNode
    {
    public:
        /// @brief The custom constructor of the ASTNode class.
        explicit ASTNode(int id, NodeType type, std::shared_ptr<ASTNode> parent) : _id(id), _parent(parent)
        {
            if (type == NodeType::CHCLASS)
                _nodeData = std::make_unique<ASTChClassNodeData>();
            else
                _nodeData = std::make_unique<ASTNodeData>();

            _nodeData->SetNodeType(type);
        }

        /// @brief The default constructor of the ASTNode class.
        ASTNode() = default;

        /// @brief A default destructor of the ASTNode class.
        ~ASTNode() = default;

        /// @return Returns a number relative to the order of the current object in the AST.
        int GetId() const;

        /// @return Returns a reference to the internal ASTNodeData container of the current object.
        std::shared_ptr<ASTNodeData> &GetDataRef();

        /// @return Returns a shared pointer to the parent of the curent object.
        std::shared_ptr<ASTNode> GetParent() const;

        /// @return Returns a reference to the internal std::list of shared pointers pointing to the children of the node
        std::list<std::shared_ptr<ASTNode>> &GetChildrenRef();

        /// @brief Sets the parent of the current object to the given shared pointer
        void SetParent(std::shared_ptr<ASTNode> parentPtr);

    private:
        int _id;
        std::shared_ptr<ASTNodeData> _nodeData;
        std::shared_ptr<ASTNode> _parent;
        std::list<std::shared_ptr<ASTNode>> _children;
    };

    /// @brief Attempts to cast the given ASTNodeData object to a ASTChClassNodeData object.
    /// @return Returns the ASTChClassNodeData representation of the given ASTNodeData object.
    ASTChClassNodeData &CastToChClassNodeData(ASTNodeData &nodeData);
};

#endif