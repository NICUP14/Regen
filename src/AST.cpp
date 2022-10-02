#include "AST.h"

RegenAST::ASTChClassNodeData &RegenAST::CastToChClassNodeData(RegenAST::ASTNodeData &nodeData)
{
    if (nodeData.GetNodeType() != NodeType::CHCLASS)
        throw RegenException::InvalidNodeDataCastException();

    return static_cast<ASTChClassNodeData &>(nodeData);
}

//? ASTNodeData method definitions placeholder.

RegenAST::NodeType RegenAST::ASTNodeData::GetNodeType() const
{
    return _nodeType;
}

std::string RegenAST::ASTNodeData::GetLiteral()
{
    return _literal;
}

bool RegenAST::ASTNodeData::Empty() const
{
    return _literal.empty();
}

void RegenAST::ASTNodeData::SetNodeType(NodeType nodeType)
{
    _nodeType = nodeType;
}

void RegenAST::ASTNodeData::Append(char ch)
{
    _literal += ch;
}

void RegenAST::ASTNodeData::_setLiteral(const std::string_view &literalRO)
{
    _literal = literalRO;
}

std::string RegenAST::ASTNodeData::_getLiteral() const
{
    return _literal;
}

//? ASTChClassNodeData method definitions placeholder.

bool RegenAST::ASTChClassNodeData::Empty() const
{
    return (_chSetIntersectFlag ? _auxiliaryChSet : _primaryChSet).none();
}

std::string RegenAST::ASTChClassNodeData::GetLiteral()
{

    std::string literal;
    std::bitset<CH_SET_SIZE> const &chSetRef =
        _chSetIntersectFlag ? _auxiliaryChSet : _primaryChSet;

    literal.reserve(chSetRef.count());
    for (unsigned char chRange = 0; chRange < CH_SET_SIZE; chRange++)
        if (chSetRef.test(chRange))
            literal += chRange;

    _setLiteral(literal);

    return _getLiteral();
}

void RegenAST::ASTChClassNodeData::Append(char ch)
{
    Set(ch);
    //? This method shouldn't be called by a ASTClassNodeData object.
}

void RegenAST::ASTChClassNodeData::SetInvertFlag(bool value)
{
    _chSetInvertFlag = value;
}

void RegenAST::ASTChClassNodeData::SetIntersectFlag(bool value)
{
    _chSetIntersectFlag = value;
}

void RegenAST::ASTChClassNodeData::Reset()
{
    std::bitset<CH_SET_SIZE> &chSetRef =
        _chSetIntersectFlag ? _auxiliaryChSet : _primaryChSet;

    if (!_chSetInvertFlag)
        chSetRef.reset();
    else
        chSetRef |= ~chSetRef; //? Sets all the set's elements to true.
}

void RegenAST::ASTChClassNodeData::Set(char ch)
{
    std::bitset<CH_SET_SIZE> &chSetRef =
        _chSetIntersectFlag ? _auxiliaryChSet : _primaryChSet;

    //? Checks if the character is already present in the set.
    if (chSetRef.test(ch) != _chSetInvertFlag)
        RegenOutput::FMTPrintWarning(fmt::format(
            RegenOutput::WarningMessage::CHCLASS_DUPLICATE_CHARACTER, ch));

    chSetRef.set(ch, !_chSetInvertFlag);
}

void RegenAST::ASTChClassNodeData::Set(const std::string_view &strRO)
{
    for (char ch : strRO)
        Set(ch);
}

void RegenAST::ASTChClassNodeData::Set(char startCh, char stopCh)
{
    if (startCh > stopCh)
    {
        if (REGEN_REGEX_COMPLIANT_FLAG)
            throw RegenException::InvalidRegexRangeException();

        std::swap(startCh, stopCh);
    }

    for (unsigned char chRange = startCh; chRange <= stopCh; chRange++)
        Set(chRange);
}

void RegenAST::ASTChClassNodeData::Normalize()
{
    if (_chSetIntersectFlag)
        _primaryChSet &= _auxiliaryChSet; //? Intersects the two character sets.

    _chSetIntersectFlag = false;
    _chSetInvertFlag = false;
    _auxiliaryChSet.reset();
}

//? ASTNode method definitions placeholder.

int RegenAST::ASTNode::GetId() const
{
    return _id;
}

std::shared_ptr<RegenAST::ASTNodeData> &RegenAST::ASTNode::GetDataRef()
{
    return _nodeData;
}

std::shared_ptr<RegenAST::ASTNode> RegenAST::ASTNode::GetParent() const
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
