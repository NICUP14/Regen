#include <Regen/Exception.h>

char const *RegenException::UnterminatedConstructException::what() const throw()
{
    return RegenOutput::ExceptionMessage::UNTERMINATED_CONSTRUCT.c_str();
}

char const *RegenException::InvalidRegexRangeException::what() const throw()
{
    return RegenOutput::ExceptionMessage::INVALID_REGEX_RANGE.c_str();
}

char const *RegenException::InvalidNodeDataCastException::what() const throw()
{
    return RegenOutput::ExceptionMessage::INVALID_NODE_DATA_CAST.c_str();
}