#include <string>
#include <bitset>
#include <memory>
#include <Regen/Output.h>
#include <Regen/Exception.h>
#include <Regen/NodeData.h>

#ifndef REGEN_CH_CLASS_NODE_DATA_HEADER
#define REGEN_CH_CLASS_NODE_DATA_HEADER

/// @brief  Encapsulates all the AST-related methods and custom data types.
namespace RegenAST
{
    /// @brief Represents a container used for storing specialized information about a node of type character class.
    class ChClassNodeData : public NodeData
    {
    public:
        /// @brief The default constructor of the ChClassNodeData class.
        ChClassNodeData();

        /// @brief The custom destructor of the ChClassNodeData class.
        ~ChClassNodeData() override = default;

        /// @returns Returns whether the current object's data container is empty.
        bool Empty() const override;

        /// @return Returns the string representation of the current object.
        std::string GetLiteral() override;

        /// @brief Overrides the base method to provide an alias for Set(char).
        void Append(char ch) override;

        /// @brief Sets the character set invert flag to the specified value.
        void SetInvertFlag(bool value);

        /// @brief Sets the character set intersect flag (abstraction) to the specified value.
        void SetIntersectFlag(bool value);

        /// @brief Resets all the vales of the set in accordance with the invert flag.
        void Reset() const;

        /// @brief Resets the value at the given character of the set in accordance with the invert flag.
        void Reset(char ch) const;

        /// @brief Sets the set's values corresponding the given character in accordance with the invert flag.
        void Set(char ch) const;

        /// @brief Sets the set's values corresponding the characters in the given string in accordance with the invert flag.
        void Set(const std::string_view &strRO) const;

        /// @brief Sets the set's values corresponding the characters in the specified range in accordance with the invert flag.
        void Set(char startCh, char stopCh) const;

        /// @brief Merges the two character sets and resets the flags.
        void Normalize();

    private:
        /// @brief Defines the fixed size of a character set.
        static const size_t CH_SET_SIZE = 255;

        bool _invertFlag = false;
        std::weak_ptr<std::bitset<CH_SET_SIZE>> _currChSetPtr;
        std::shared_ptr<std::bitset<CH_SET_SIZE>> _primaryChSetPtr;
        std::shared_ptr<std::bitset<CH_SET_SIZE>> _auxiliaryChSetPtr = nullptr;
    };

    /// @brief Attempts to cast the given NodeData object to a ChClassNodeData object.
    /// @return Returns the ChClassNodeData representation of the given NodeData object.
    ChClassNodeData &CastToChClassNodeData(RegenAST::NodeData &nodeData);
}

#endif