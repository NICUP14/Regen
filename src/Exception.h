#include <exception>

namespace RegenException
{
    class ExceptionMessage
    {
    public:
        static const char *contextMismatch;
    };

    class ContextMismatchException : public std::exception
    {
    public:
        char const *what() const throw() override;
    };
};