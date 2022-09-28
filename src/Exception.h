#include <string>
#include <exception>
#include <fmt/core.h>
#include "Output.h"

#ifndef REGEN_EXCEPTION_HEADER_H
#define REGEN_EXCEPTION_HEADER_H

/// @brief  Encapsulates all the custom exceptions messages and definitions.
namespace RegenException
{
    class UnterminatedConstructException : public std::exception
    {
    public:
        explicit UnterminatedConstructException(std::string ctxGrStrRO)
        {
            _message = fmt::format(
                RegenOutput::ExceptionMessage::UNTERMINATED_CONSTRUCT,
                ctxGrStrRO);
        }
        char const *what() const throw() override;

    private:
        std::string _message;
    };

    class InvalidRegexRangeException : public std::exception
    {
    public:
        InvalidRegexRangeException() = default;
        char const *what() const throw() override;
    };

    class NodeDataMismatchException : public std::exception
    {
    public:
        explicit NodeDataMismatchException(std::string expectedNodeTypeStrRO, const std::string_view &invalidNodeTypeRO)
        {
            _message = fmt::format(
                RegenOutput::ExceptionMessage::NODE_DATA_MISMATCH,
                expectedNodeTypeStrRO,
                invalidNodeTypeRO);
        }
        char const *what() const throw() override;

    private:
        std::string _message;
    };
};

#endif