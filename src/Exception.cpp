#include "Exception.h"

// TODO: Assign the exception messages to something more suggestive.

//? Exception messages declarations
const char *RegenException::ExceptionMessage::ContextMismatch = "ContextMismatchException";
const char *RegenException::ExceptionMessage::InvalidRange = "InvalidRangeException";
const char *RegenException::ExceptionMessage::NodeDataMismatch = "NodeDataMismatchException";

// TODO: Include variable parameters into the custom exception constructors.

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