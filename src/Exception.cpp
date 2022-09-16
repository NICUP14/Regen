#include "Exception.h"

//? Exception messages declarations
const char *RegenException::ExceptionMessage::contextMismatch = "ContextMismatchException";

//? Exception classes declarations
char const *RegenException::ContextMismatchException::what() const throw()
{
    return ExceptionMessage::contextMismatch;
}