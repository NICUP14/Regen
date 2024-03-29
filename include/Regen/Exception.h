#include <string>
#include <exception>
#include <Regen/Output.h>

#ifndef REGEN_EXCEPTION_HEADER
#define REGEN_EXCEPTION_HEADER

/// @brief  Encapsulates all the custom exception types.
namespace RegenException
{
    class UnterminatedConstructException : public std::exception
    {
    public:
        UnterminatedConstructException() = default;
        char const *what() const throw() override;
    };

    class InvalidRegexRangeException : public std::exception
    {
    public:
        InvalidRegexRangeException() = default;
        char const *what() const throw() override;
    };

    class InvalidNodeDataCastException : public std::exception
    {
    public:
        InvalidNodeDataCastException() = default;
        char const *what() const throw() override;
    };
}

#endif