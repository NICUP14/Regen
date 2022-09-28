#include "Exception.h"

// TODO: Assign the exception messages to something more suggestive.

// TODO: Include variable parameters into the custom exception constructors.

//? Exception classes declarations
char const *RegenException::UnterminatedConstructException::what() const throw()
{
    return _message.c_str();
}

char const *RegenException::InvalidRegexRangeException::what() const throw()
{
    return RegenOutput::ExceptionMessage::INVALID_REGEX_RANGE.c_str();
}

char const *RegenException::NodeDataMismatchException::what() const throw()
{
    return _message.c_str();
}