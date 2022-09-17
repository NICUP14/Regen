#include "AST.h"

void RegenAST::ASTNodeData::SetChSet(const std::string &str)
{
    if (_type != RegenAST::nodeType::CHCLASS)
        throw RegenException::NodeDataMismatchException();

    for (auto ch : str)
        _chSet.set(ch, true);
}

void RegenAST::ASTNodeData::SetChSet(char startCh, char stopCh)
{
    if (_type != RegenAST::nodeType::CHCLASS)
        throw RegenException::NodeDataMismatchException();

    if (startCh > stopCh)
        std::swap(startCh, stopCh);

    for (char ch = startCh; ch <= stopCh; ch++)
        _chSet.set(ch);
}

const std::string RegenAST::ASTNodeData::GetLiteral()
{
    if (_type != RegenAST::nodeType::LITERAL)
        throw RegenException::NodeDataMismatchException();

    return _literal;
}