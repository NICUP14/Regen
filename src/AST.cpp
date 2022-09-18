#include "AST.h"

int RegenAST::ASTNode::GetId()
{
    return _id;
}

std::shared_ptr<RegenAST::ASTNode> RegenAST::ASTNode::GetParent()
{
    return _parent;
}

std::list<std::shared_ptr<RegenAST::ASTNode>> &RegenAST::ASTNode::GetChildren()
{
    return _children;
}

void RegenAST::ASTNode::setParent(std::shared_ptr<RegenAST::ASTNode> parent)
{
    _parent = parent;
}

bool RegenAST::ASTNodeData::Empty()
{
    if (_type == RegenAST::nodeType::LITERAL)
        return _literal.empty();
    else if (_type == RegenAST::nodeType::CHCLASS)
        return _chSet.none();

    //? Fuck IDE warnings
    return true;
}

RegenAST::nodeType RegenAST::ASTNodeData::GetNodeType()
{
    return _type;
}

std::string RegenAST::ASTNodeData::GetLiteral()
{
    if (_type == RegenAST::nodeType::CHCLASS)
    {
        _literal.reserve(_chSet.count());
        for (size_t ch = 0; ch < _chSet.size(); ch++)
            if (_chSet.test(ch))
                _literal += (char)ch;
    }

    return _literal;
}

void RegenAST::ASTNodeData::setNodeType(RegenAST::nodeType type)
{
    _type = type;
}

void RegenAST::ASTNodeData::SetLiteral(const std::string &str)
{
    _literal = str;
}

void RegenAST::ASTNodeData::FillChSet(bool value)
{
    if (_type != RegenAST::nodeType::CHCLASS)
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
    if (_type != RegenAST::nodeType::CHCLASS)
        throw RegenException::NodeDataMismatchException();

    _chSet.set(ch, !invertFlag);
}

void RegenAST::ASTNodeData::SetChSet(const std::string &str, bool invertFlag)
{
    if (_type != RegenAST::nodeType::CHCLASS)
        throw RegenException::NodeDataMismatchException();

    for (auto ch : str)
        _chSet.set(ch, !invertFlag);
}

void RegenAST::ASTNodeData::SetChSet(char startCh, char stopCh, bool invertFlag)
{
    if (_type != RegenAST::nodeType::CHCLASS)
        throw RegenException::NodeDataMismatchException();

    if (startCh > stopCh)
        std::swap(startCh, stopCh);

    for (char chRange = startCh; chRange <= stopCh; chRange++)
        _chSet.set(chRange, !invertFlag);
}