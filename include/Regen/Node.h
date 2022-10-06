#include <list>
#include <memory>
#include <Regen/NodeData.h>
#include <Regen/ChClassNodeData.h>

#ifndef REGEN_NODE_HEADER
#define REGEN_NODE_HEADER

/// @brief  Encapsulates all the AST-related methods and custom data types.
namespace RegenAST
{
    /// @brief Represents a node of the Regen AST.
    class Node
    {
    public:
        /// @brief The custom constructor of the Node class.
        explicit Node(int id, NodeData::Type type, std::shared_ptr<Node> parent) : _id(id), _parentPtr(parent)
        {
            if (type == NodeData::Type::CHCLASS)
                _nodeDataPtr = std::make_unique<RegenAST::ChClassNodeData>();
            else
                _nodeDataPtr = std::make_unique<NodeData>();

            _nodeDataPtr->SetType(type);
        }

        /// @brief A default destructor of the Node class.
        ~Node() = default;

        /// @return Returns a number relative to the order of the current object in the AST.
        int GetId() const;

        /// @return Returns a reference to the internal NodeData container of the current object.
        std::shared_ptr<NodeData> GetNodeDataPtr() const;

        /// @return Returns a shared pointer to the parent of the curent object.
        std::shared_ptr<Node> GetParentPtr() const;

        /// @return Returns a reference to the internal std::list of shared pointers pointing to the children of the node
        std::list<std::shared_ptr<Node>> &GetChildrenListRef();

        /// @brief Sets the parent of the current object to the given shared pointer
        void SetParentPtr(std::shared_ptr<Node> parentPtr);

    private:
        int _id;
        std::shared_ptr<Node> _parentPtr;
        std::shared_ptr<NodeData> _nodeDataPtr;
        std::list<std::shared_ptr<Node>> _childrenList;
    };

}

#endif