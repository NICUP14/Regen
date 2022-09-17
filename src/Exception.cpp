#include "Exception.h"

//? Exception messages declarations
const char *RegenException::ExceptionMessage::ContextMismatch = "ContextMismatchException";
const char *RegenException::ExceptionMessage::InvalidRange = "InvalidRangeException";
const char *RegenException::ExceptionMessage::NodeDataMismatch = "NodeDataMismatchException";

//? Exception classes declarations
char const *RegenException::ContextMismatchException::what() const throw()
{
    return ExceptionMessage::ContextMismatch;
}

char const *RegenException::InvalidRangeException::what() const throw()
{
    return ExceptionMessage::InvalidRange;
}

char const *RegenException::NodeDataMismatchException::what() const throw()
{
    return ExceptionMessage::NodeDataMismatch;
}