#include "Output.h"

void RegenOutput::CoutSuccess(const std::string &message)
{
    fmt::print(fmt::fg(fmt::color::green), "{}\n", message);
}

void RegenOutput::CerrWarning(const std::string &message)
{
    fmt::print(fmt::fg(fmt::color::purple), "{}\n", message);
}

void RegenOutput::CerrError(const std::string &message)
{
    fmt::print(fmt::fg(fmt::color::red), "{}\n", message);
}