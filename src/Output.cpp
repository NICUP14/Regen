#include "Output.h"

//? Expected fmt::print parameters: expressionCh
const std::string RegenOutput::WarningMessage::CHCLASS_DUPLICATE_CHARACTER =
    "The parser detected the redundant character '{}' inside the custom character class in the given expression";

//? Expected fmt::print parameters: None
const std::string RegenOutput::WarningMessage::CHCLASS_EMPTY =
    "The parser detected a possibly redundant empty character class in the given expression";

//? Expected fmt::print parameters: nodeDataLiteral
const std::string RegenOutput::WarningMessage::CHCLASS_REDUNDANT_INIT =
    "The parser detected the redundant custom character class initializer \"{}\" in the given expression";

//? Expected fmt::print parameters: None
const std::string RegenOutput::WarningMessage::CHCLASS_MISLEADING_RANGE =
    "The parser detected a possibly misleading range-like (fake range) custom character class initializer";

//? Expected fmt::print parameters: None
const std::string RegenOutput::ExceptionMessage::UNTERMINATED_CONSTRUCT =
    "The specified Regen expression contains an unterminated/invalid construct";

//? Expected fmt::print parameters: None
const std::string RegenOutput::ExceptionMessage::INVALID_REGEX_RANGE =
    "The requested range operation is invalid due to the regex compliancy flag";

//? Expected fmt::print parameters: None
const std::string RegenOutput::ExceptionMessage::INVALID_NODE_DATA_CAST =
    "The attempted cast from a ASTNodeData object to a ASTChClassNodeData object is invalid";

void RegenOutput::FMTPrintDebug(const std::string &message)
{
    if (OUTPUT_ENABLED_FLAG)
        fmt::print(fmt::fg(fmt::color::blue), "DEBUG: {}\n", message);
}

void RegenOutput::FMTPrintSuccess(const std::string &message)
{
    if (OUTPUT_ENABLED_FLAG)
        fmt::print(fmt::fg(fmt::color::green), "SUCCESS: {}\n", message);
}

void RegenOutput::FMTPrintWarning(const std::string &message)
{
    if (OUTPUT_ENABLED_FLAG)
        fmt::print(stderr, fmt::fg(fmt::color::purple), "WARNING: {}\n", message);
}

void RegenOutput::FMTPrintError(const std::string &message)
{
    if (OUTPUT_ENABLED_FLAG)
        fmt::print(stderr, fmt::fg(fmt::color::red), "ERROR: {}\n", message);
}