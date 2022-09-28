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
    if (_nodeType == NodeType::LITERAL)
        return _literal.empty();
    else if (_nodeType == NodeType::CHCLASS)
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
    if (_nodeType == NodeType::CHCLASS)
    {
        _literal.reserve(_chSet.count());

        for (size_t chSetIdx = 0; chSetIdx < _chSet.size(); chSetIdx++)
            if (_chSet.test(chSetIdx))
                _literal += (char)chSetIdx;
    }

    return _literal;
}

void RegenAST::ASTNodeData::SetInvertFlag(bool value)
{
    if (_nodeType != NodeType::CHCLASS)
        throw RegenException::NodeDataMismatchException(
            NodeTypeToStr(NodeType::CHCLASS),
            NodeTypeToStr(_nodeType));

    _chSetInvertFlag = value;
}

void RegenAST::ASTNodeData::SetNodeType(NodeType type)
{
    _nodeType = type;
}

void RegenAST::ASTNodeData::SetLiteral(const std::string &str)
{
    //? Type checker placeholder. (#1)
    if (_nodeType != NodeType::LITERAL)
        throw RegenException::NodeDataMismatchException(
            NodeTypeToStr(NodeType::LITERAL),
            NodeTypeToStr(_nodeType));

    _literal = str;
}

void RegenAST::ASTNodeData::FillChSet(bool value)
{
    //? Type checker placeholder. (#2)
    if (_nodeType != NodeType::CHCLASS)
        throw RegenException::NodeDataMismatchException(
            NodeTypeToStr(NodeType::CHCLASS),
            NodeTypeToStr(_nodeType));

    if (!value)
    {
        _chSet.reset();
        return;
    }

    for (unsigned char chRange = 0; chRange < (unsigned char)CH_SET_SIZE; chRange++)
        _chSet.set(chRange);
}

void RegenAST::ASTNodeData::SetChSet(const std::string_view &strRO)
{
    // TODO: Print a warning about duplicate characters in the string.
    //? Type checker placeholder. (#4)
    if (_nodeType != NodeType::CHCLASS)
        throw RegenException::NodeDataMismatchException(
            NodeTypeToStr(NodeType::CHCLASS),
            NodeTypeToStr(_nodeType));

    for (auto ch : strRO)
    {
        if (RegenOutput::OUTPUT_ENABLED &&
            _chSet.test(ch) != _chSetInvertFlag)
            RegenOutput::FMTPrintWarning(fmt::format(RegenOutput::WarningMessage::CHCLASS_DUPLICATE_CHARACTER, ch));
        _chSet.set(ch, !_chSetInvertFlag);
    }
}

void RegenAST::ASTNodeData::SetChSet(char startCh, char stopCh)
{
    //? Type checker placeholder. (#5)
    if (_nodeType != NodeType::CHCLASS)
        throw RegenException::NodeDataMismatchException(
            NodeTypeToStr(NodeType::CHCLASS),
            NodeTypeToStr(_nodeType));

    //? "It just works"
    if (REGEN_REGEX_COMPLIANT_FLAG && startCh > stopCh)
        throw RegenException::InvalidRegexRangeException();

    if (startCh > stopCh)
        std::swap(startCh, stopCh);

    for (char ch = startCh; ch <= stopCh; ch++)
    {
        if (RegenOutput::OUTPUT_ENABLED &&
            _chSet.test(ch) != _chSetInvertFlag)
            RegenOutput::FMTPrintWarning(fmt::format(RegenOutput::WarningMessage::CHCLASS_DUPLICATE_CHARACTER, ch));
        _chSet.set(ch, !_chSetInvertFlag);
    }
}