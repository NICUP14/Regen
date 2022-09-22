#include "Output.h"

void RegenOutput::FMTPrintSuccess(const std::string &message)
{
    fmt::print(fmt::fg(fmt::color::green), "{}\n", message);
}

void RegenOutput::FMTPrintWarning(const std::string &message)
{
    fmt::print(stderr, fmt::fg(fmt::color::purple), "{}\n", message);
}

void RegenOutput::FMTPrintError(const std::string &message)
{
    fmt::print(stderr, fmt::fg(fmt::color::red), "{}\n", message);
}