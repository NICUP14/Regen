#include <Regen/Node.h>

RegenAST::NodeData::Type RegenAST::NodeData::GetType() const
{
    return _type;
}

std::string RegenAST::NodeData::GetLiteral()
{
    return _getLiteral();
}

bool RegenAST::NodeData::Empty() const
{
    return _literal.empty();
}

void RegenAST::NodeData::SetType(Type type)
{
    _type = type;
}

void RegenAST::NodeData::Append(char ch)
{
    _literal += ch;
}

std::string RegenAST::NodeData::_getLiteral() const
{
    return _literal;
}

void RegenAST::NodeData::_setLiteral(const std::string_view &literalRO)
{
    _literal = literalRO;
}