#include "AST.h"

int RegenAST::ASTNode::GetId() const
{
    return _id;
}

RegenAST::ASTNodeData &RegenAST::ASTNode::GetDataRef()
{
    return _nodeData;
}

std::shared_ptr<RegenAST::ASTNode> RegenAST::ASTNode::GetParentRef()
{
    return _parent;
}

std::list<std::shared_ptr<RegenAST::ASTNode>> &RegenAST::ASTNode::GetChildrenRef()
{
    return _children;
}

void RegenAST::ASTNode::SetParent(std::shared_ptr<RegenAST::ASTNode> parent)
{
    _parent = parent;
}

bool RegenAST::ASTNodeData::Empty() const
{
    if (_nodeType == RegenAST::NodeType::LITERAL)
        return _literal.empty();
    else if (_nodeType == RegenAST::NodeType::CHCLASS)
        return _chSet.none();

    /// Omitting this return statement will trigger the compiler.
    return true;
}

RegenAST::NodeType RegenAST::ASTNodeData::GetNodeType() const
{
    return _nodeType;
}

std::string RegenAST::ASTNodeData::GetLiteral()
{
    /// Appends all the characters present in the set to literal.
    if (_nodeType == RegenAST::NodeType::CHCLASS)
    {
        _literal.reserve(_chSet.count());

        for (size_t chSetIdx = 0; chSetIdx < _chSet.size(); chSetIdx++)
            if (_chSet.test(chSetIdx))
                _literal += (char)chSetIdx;
    }

    return _literal;
}

void RegenAST::ASTNodeData::SetNodeType(RegenAST::NodeType type)
{
    _nodeType = type;
}

void RegenAST::ASTNodeData::SetLiteral(const std::string &str)
{
    //? Type checker placeholder. (#1)
    if (_nodeType != RegenAST::NodeType::LITERAL)
        throw RegenException::NodeDataMismatchException();

    _literal = str;
}

void RegenAST::ASTNodeData::FillChSet(bool value)
{
    //? Type checker placeholder. (#2)
    if (_nodeType != RegenAST::NodeType::CHCLASS)
        throw RegenException::NodeDataMismatchException();

    if (!value)
    {
        _chSet.reset();
        return;
    }

    for (unsigned char chRange = 0; chRange < (unsigned char)RegenAST::CH_SET_SIZE; chRange++)
        _chSet.set(chRange);
}

void RegenAST::ASTNodeData::SetChSet(char ch, bool invertFlag)
{
    //? Type checker placeholder. (#3)
    if (_nodeType != RegenAST::NodeType::CHCLASS)
        throw RegenException::NodeDataMismatchException();

    _chSet.set(ch, !invertFlag);
}

void RegenAST::ASTNodeData::SetChSet(const std::string_view &strRO, bool invertFlag)
{
    //? Type checker placeholder. (#4)
    if (_nodeType != RegenAST::NodeType::CHCLASS)
        throw RegenException::NodeDataMismatchException();

    for (auto ch : strRO)
        _chSet.set(ch, !invertFlag);
}

void RegenAST::ASTNodeData::SetChSet(char startCh, char stopCh, bool invertFlag)
{
    //? Type checker placeholder. (#5)
    if (_nodeType != RegenAST::NodeType::CHCLASS)
        throw RegenException::NodeDataMismatchException();

    if (startCh > stopCh)
        std::swap(startCh, stopCh);

    for (char chRange = startCh; chRange <= stopCh; chRange++)
        _chSet.set(chRange, !invertFlag);
}