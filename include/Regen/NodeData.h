#include <string>

#ifndef REGEN_NODE_DATA_HEADER
#define REGEN_NODE_DATA_HEADER

/// @brief  Encapsulates all the AST-related methods and custom data types.
namespace RegenAST
{
    /// @brief Represents a container used for storing general information about a node.
    class NodeData
    {
    public:
        /// @brief Defines all the possible types of nodes.
        enum class Type
        {
            ENTRY,
            LITERAL,
            CHCLASS,
        };

        /// @brief The virtual destructor of the NodeData class.
        virtual ~NodeData() = default;

        /// @return Returns the type of node of the current object.
        Type GetType() const;

        /// @returns Returns whether the current object's data is empty.
        virtual bool Empty() const;

        /// @return Returns the string representation of the current object.
        virtual std::string GetLiteral();

        /// @brief Sets the node type of the current object to the given type.
        void SetType(Type type);

        /// @brief Appends the given character to the current object's container.
        virtual void Append(char ch);

    private:
        Type _type;
        std::string _literal;

    protected:
        /// @brief Provides derived classes to bypass the overriden GetLiteral method.
        /// @return Returns the same value returned by the GetLiteral method.
        std::string _getLiteral() const;

        /// @brief Sets the given string as the new container of the current object.
        void _setLiteral(const std::string_view &literalRO);
    };
}

#endif