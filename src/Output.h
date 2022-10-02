#include <iostream>
#include <string>

#define FMT_HEADER_ONLY
#include <fmt/color.h>

#ifndef REGEN_OUTPUT_HEADER_H
#define REGEN_OUTPUT_HEADER_H

/// @brief Encapsulates all the custom exception/warning messages and methods.
namespace RegenOutput
{
    const bool OUTPUT_ENABLED_FLAG = true;

    /// @brief Encapsulates all the Regen warning messages.
    struct WarningMessage
    {
        static const std::string CHCLASS_EMPTY;
        static const std::string CHCLASS_REDUNDANT_INIT;
        static const std::string CHCLASS_DUPLICATE_CHARACTER;
        static const std::string CHCLASS_MISLEADING_RANGE;
    };

    /// @brief Encapsulates all the Regen exception messages.
    struct ExceptionMessage
    {
        static const std::string INVALID_NODE_DATA_CAST;
        static const std::string INVALID_CONTEXT_GROUP;
        static const std::string INVALID_REGEX_RANGE;
        static const std::string NODE_DATA_MISMATCH;
        static const std::string UNTERMINATED_CONSTRUCT;
    };

    void FMTPrintDebug(const std::string &message);

    void FMTPrintSuccess(const std::string &message);

    void FMTPrintWarning(const std::string &message);

    void FMTPrintError(const std::string &message);
};

#endif