#include "Output.h"

void RegenOutput::CoutSuccess(const std::string &message)
{
    fmt::print(fmt::fg(fmt::color::green), "{}\n", message);
    // std::cout << ANSI_GREEN << message << ANSI_RESET << '\n';
}

void RegenOutput::CerrWarning(const std::string &message)
{
    fmt::print(fmt::fg(fmt::color::purple), "{}\n", message);
    // std::cerr << ANSI_PURPLE << message << ANSI_RESET << '\n';
}

void RegenOutput::CerrError(const std::string &message)
{
    fmt::print(fmt::fg(fmt::color::red), "{}\n", message);
    // std::cerr << ANSI_RED << message << ANSI_RESET << '\n';
}