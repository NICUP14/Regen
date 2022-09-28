#include "Output.h"

//? Expected fmt::print parameters: expressionCh
const std::string RegenOutput::WarningMessage::CHCLASS_DUPLICATE_CHARACTER =
    "The parser detected the redundant character '{}' inside the custom character class in the given expression";

//? Expected fmt::print parameters: None
const std::string RegenOutput::WarningMessage::CHCLASS_EMPTY =
    "The parser detected a possibly redundant empty character class in the given expression";

const std::string RegenOutput::WarningMessage::CHCLASS_MISLEADING_RANGE =
    "The parser detected a possibly misleading range-like (fake range) custom character class";

//? Expected fmt::print parameters: contextGroupStr
const std::string RegenOutput::ExceptionMessage::UNTERMINATED_CONSTRUCT =
    "The specified regen expression contains an unterminated/invalid construct (contextGroup = {})";

//? Expected fmt::print parameters: REGEN_REGEX_COMPLIANT_FLAG
const std::string RegenOutput::ExceptionMessage::INVALID_REGEX_RANGE =
    "The requested range operation is invalid due to the regex compliancy flag (REGEN_REGEX_COMPLIANT_FLAG = {})";

//? Expected fmt::print parameters: nodeId, expectedNodeType, invalidNodeType
const std::string RegenOutput::ExceptionMessage::NODE_DATA_MISMATCH =
    "The internal type of the node is invalid (expected {}; got {})";

void RegenOutput::FMTPrintDebug(const std::string &message)
{
    fmt::print(fmt::fg(fmt::color::blue), "DEBUG: {}\n", message);
}

void RegenOutput::FMTPrintSuccess(const std::string &message)
{
    fmt::print(fmt::fg(fmt::color::green), "SUCCESS: {}\n", message);
}

void RegenOutput::FMTPrintWarning(const std::string &message)
{
    fmt::print(stderr, fmt::fg(fmt::color::purple), "WARNING: {}\n", message);
}

void RegenOutput::FMTPrintError(const std::string &message)
{
    fmt::print(stderr, fmt::fg(fmt::color::red), "ERROR: {}\n", message);
}