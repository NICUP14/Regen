#include <list>
#include <bitset>
#include <algorithm>
#include <functional>
#include "Exception.h"

#ifndef REGEN_AST_HEADER_H
#define REGEN_AST_HEADER_H

namespace RegenAST
{
    enum class nodeType
    {
        ENTRY,
        LITERAL,
        CHCLASS
    };

    const short BIT_SIZE = 8;
    const size_t CH_SET_SIZE = sizeof(wchar_t) * BIT_SIZE;

    class ASTNodeData
    {
    public:
        nodeType GetNodeType();

        //? Additional checking with type
        void SetChSet(const std::string &str);
        void SetChSet(char startCh, char stopCh);
        // std::bitset<CH_SET_SIZE> &GetChSet();
        const std::string GetLiteral();

    private:
        nodeType _type;
        std::string _literal;
        std::bitset<CH_SET_SIZE> _chSet;
    };

    struct ASTNode
    {
        explicit ASTNode(nodeType type, ASTNode &parent) : Type(type), Parent(parent) {}
        ASTNode() = default;
        ~ASTNode() = default;

        nodeType Type;
        ASTNodeData Data;
        std::list<ASTNode> Children;
        std::reference_wrapper<ASTNode> Parent;
    };

};

#endif