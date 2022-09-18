#include <list>
#include <bitset>
#include <algorithm>
#include <memory>
#include <functional>
#include "Exception.h"

// TODO: Remove iostream import
#include <iostream>

#ifndef REGEN_AST_HEADER_H
#define REGEN_AST_HEADER_H

namespace RegenAST
{
    enum class nodeType
    {
        ENTRY,
        LITERAL,
        CHCLASS,
    };

    const size_t CH_SET_SIZE = 255;

    class ASTNodeData
    {
    public:
        //? Additional checking with type
        bool Empty();
        nodeType GetNodeType();
        std::string GetLiteral();
        void setNodeType(nodeType type);
        void SetLiteral(const std::string &str);
        void FillChSet(bool value = false);
        void SetChSet(char ch, bool invertFlag = false);
        void SetChSet(const std::string &str, bool invertFlag = false);
        void SetChSet(char startCh, char stopCh, bool invertFlag = false);

    private:
        nodeType _type;
        std::bitset<CH_SET_SIZE> _chSet;
        std::string _literal;
    };

    struct ASTNode
    {
    public:
        explicit ASTNode(int id, nodeType type, std::shared_ptr<ASTNode> parent) : _id(id), _parent(parent) { Data.setNodeType(type); }
        ASTNode() = default;
        ~ASTNode() = default;

        int GetId();
        std::shared_ptr<ASTNode> GetParent();
        std::list<std::shared_ptr<ASTNode>> &GetChildren();
        void setParent(std::shared_ptr<ASTNode> parent);

        ASTNodeData Data;

    private:
        int _id;
        std::shared_ptr<ASTNode> _parent;
        std::list<std::shared_ptr<ASTNode>> _children;
    };

};

#endif