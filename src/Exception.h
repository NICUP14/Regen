#include <exception>

#ifndef REGEN_EXCEPTION_HEADER_H
#define REGEN_EXCEPTION_HEADER_H

namespace RegenException
{
    struct ExceptionMessage
    {
        static const char *ContextMismatch;
        static const char *InvalidRange;
        static const char *NodeDataMismatch;
    };

    class ContextMismatchException : public std::exception
    {
    public:
        char const *what() const throw() override;
    };

    class InvalidRangeException : public std::exception
    {
    public:
        char const *what() const throw() override;
    };

    class NodeDataMismatchException : public std::exception
    {
    public:
        char const *what() const throw() override;
    };
};

#endif