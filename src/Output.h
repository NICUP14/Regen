#include <iostream>
#include <string>

#define FMT_HEADER_ONLY
#include <fmt/color.h>

#ifndef REGEN_OUTPUT_HEADER_H
#define REGEN_OUTPUT_HEADER_H

namespace RegenOutput
{
    const bool OUTPUT_ENABLED = true;

    const std::string ANSI_RESET = R"(\e[0m)";
    const std::string ANSI_RED = R"(\e[0;31m)";
    const std::string ANSI_GREEN = R"(\e[0;32m)";
    const std::string ANSI_PURPLE = R"(\e[0;35m)";

    const std::string CHCLASS_EMPTY_WARNING = "CHCLASS_EMPTY_WARNING";
    const std::string NCHCLASS_EMPTY_WARNING = "CHCLASS_EMPTY_WARNING";

    void CoutSuccess(const std::string &message);
    void CerrWarning(const std::string &message);
    void CerrError(const std::string &message);
};

#endif