#include <Regen/ChClassNodeData.h>

RegenAST::ChClassNodeData::ChClassNodeData()
{
    SetType(Type::CHCLASS);

    _primaryChSetPtr = std::make_shared<std::bitset<CH_SET_SIZE>>();
    _currChSetPtr = _primaryChSetPtr;
}

bool RegenAST::ChClassNodeData::Empty() const
{
    return (_currChSetPtr.lock())->none();
}

std::string RegenAST::ChClassNodeData::GetLiteral()
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

void RegenAST::ChClassNodeData::Append(char ch)
{
    Set(ch);
}

void RegenAST::ChClassNodeData::SetInvertFlag(bool value)
{
    _invertFlag = value;
}

void RegenAST::ChClassNodeData::SetIntersectFlag(bool value)
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

void RegenAST::ChClassNodeData::Reset() const
{
    auto currChSetPtr = _currChSetPtr.lock();

    if (!_invertFlag)
        currChSetPtr->reset();
    else
        *currChSetPtr |= ~(*currChSetPtr); //? Sets all the set's elements to true.
}

void RegenAST::ChClassNodeData::Reset(char ch) const
{
    _currChSetPtr.lock()->set(ch, _invertFlag);
}

void RegenAST::ChClassNodeData::Set(char ch) const
{
    auto currChSetPtr = _currChSetPtr.lock();

    //? Checks if the character is already present in the set.
    if (currChSetPtr->test(ch) != _invertFlag)
        RegenOutput::FMTPrintWarning(fmt::format(
            RegenOutput::WarningMessage::CHCLASS_DUPLICATE_CHARACTER, ch));

    currChSetPtr->set(ch, !_invertFlag);
}

void RegenAST::ChClassNodeData::Set(const std::string_view &strRO) const
{
    for (char ch : strRO)
        Set(ch);
}

void RegenAST::ChClassNodeData::Set(char startCh, char stopCh) const
{
    for (unsigned char chRange = startCh; chRange <= stopCh; chRange++)
        Set(chRange);
}

void RegenAST::ChClassNodeData::Normalize()
{
    if (_auxiliaryChSetPtr != nullptr)
        *_primaryChSetPtr &= *_auxiliaryChSetPtr;

    _invertFlag = false;
    _auxiliaryChSetPtr = nullptr;
    _currChSetPtr = _primaryChSetPtr;
}

RegenAST::ChClassNodeData &RegenAST::CastToChClassNodeData(RegenAST::NodeData &nodeData)
{
    if (nodeData.GetType() != RegenAST::NodeData::Type::CHCLASS)
        throw RegenException::InvalidNodeDataCastException();

    return static_cast<ChClassNodeData &>(nodeData);
}
