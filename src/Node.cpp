#include <Regen/Node.h>

int RegenAST::Node::GetId() const
{
    return _id;
}

std::shared_ptr<RegenAST::NodeData> RegenAST::Node::GetNodeDataPtr() const
{
    return _nodeDataPtr;
}

std::shared_ptr<RegenAST::Node> RegenAST::Node::GetParentPtr() const
{
    return _parentPtr;
}

std::list<std::shared_ptr<RegenAST::Node>> &RegenAST::Node::GetChildrenListRef()
{
    return _childrenList;
}

void RegenAST::Node::SetParentPtr(std::shared_ptr<RegenAST::Node> parentPtr)
{
    _parentPtr = parentPtr;
}