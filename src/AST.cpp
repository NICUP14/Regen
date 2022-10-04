#include <Regen/AST.h>

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
    return _getLiteral();
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

std::string RegenAST::ASTNodeData::_getLiteral() const
{
    return _literal;
}

void RegenAST::ASTNodeData::_setLiteral(const std::string_view &literalRO)
{
    _literal = literalRO;
}

//? ASTChClassNodeData method definitions placeholder.

RegenAST::ASTChClassNodeData::ASTChClassNodeData()
{
    SetNodeType(NodeType::CHCLASS);

    _primaryChSetPtr = std::make_shared<std::bitset<CH_SET_SIZE>>();
    _currChSetPtr = _primaryChSetPtr;
}

bool RegenAST::ASTChClassNodeData::Empty() const
{
    return (_currChSetPtr.lock())->none();
}

std::string RegenAST::ASTChClassNodeData::GetLiteral()
{
    std::string literal;
    auto currChSetPtr = _currChSetPtr.lock();

    literal.reserve(currChSetPtr->count());
    for (unsigned char chRange = 0; chRange < CH_SET_SIZE; chRange++)
        if (currChSetPtr->test(chRange))
            literal += chRange;

    _setLiteral(literal);

    return _getLiteral();
}

void RegenAST::ASTChClassNodeData::Append(char ch)
{
    Set(ch);
}

void RegenAST::ASTChClassNodeData::SetInvertFlag(bool value)
{
    _invertFlag = value;
}

void RegenAST::ASTChClassNodeData::SetIntersectFlag(bool value)
{
    if (!value)
    {
        _currChSetPtr = _primaryChSetPtr;
        return;
    }
    else
    {
        if (_auxiliaryChSetPtr == nullptr)
            _auxiliaryChSetPtr = std::make_shared<std::bitset<CH_SET_SIZE>>();

        _currChSetPtr = _auxiliaryChSetPtr;
    }
}

void RegenAST::ASTChClassNodeData::Reset() const
{
    auto currChSetPtr = _currChSetPtr.lock();

    if (!_invertFlag)
        currChSetPtr->reset();
    else
        *currChSetPtr |= ~(*currChSetPtr); //? Sets all the set's elements to true.
}

void RegenAST::ASTChClassNodeData::Reset(char ch) const
{
    _currChSetPtr.lock()->set(ch, _invertFlag);
}

void RegenAST::ASTChClassNodeData::Set(char ch) const
{
    auto currChSetPtr = _currChSetPtr.lock();

    //? Checks if the character is already present in the set.
    if (currChSetPtr->test(ch) != _invertFlag)
        RegenOutput::FMTPrintWarning(fmt::format(
            RegenOutput::WarningMessage::CHCLASS_DUPLICATE_CHARACTER, ch));

    currChSetPtr->set(ch, !_invertFlag);
}

void RegenAST::ASTChClassNodeData::Set(const std::string_view &strRO) const
{
    for (char ch : strRO)
        Set(ch);
}

void RegenAST::ASTChClassNodeData::Set(char startCh, char stopCh) const
{
    for (unsigned char chRange = startCh; chRange <= stopCh; chRange++)
        Set(chRange);
}

void RegenAST::ASTChClassNodeData::Normalize()
{
    if (_auxiliaryChSetPtr != nullptr)
        *_primaryChSetPtr &= *_auxiliaryChSetPtr;

    _invertFlag = false;
    _auxiliaryChSetPtr = nullptr;
    _currChSetPtr = _primaryChSetPtr;
}

//? ASTNode method definitions placeholder.

int RegenAST::ASTNode::GetId() const
{
    return _id;
}

std::shared_ptr<RegenAST::ASTNodeData> RegenAST::ASTNode::GetNodeDataPtr() const
{
    return _nodeDataPtr;
}

std::shared_ptr<RegenAST::ASTNode> RegenAST::ASTNode::GetParentPtr() const
{
    return _parentPtr;
}

std::list<std::shared_ptr<RegenAST::ASTNode>> &RegenAST::ASTNode::GetChildrenRef()
{
    return _childrenList;
}

void RegenAST::ASTNode::SetParent(std::shared_ptr<RegenAST::ASTNode> parentPtr)
{
    _parentPtr = parentPtr;
}
