#ifndef COMPUTING_LEXER
#define COMPUTING_LEXER

#include <string>
#include <list>
#include <vector>

#include "token.h"

namespace Computing
{

class Lexer
{
    public:
        std::list<Token*> Split(const std::string&) noexcept;
        bool PrintErrors(void) noexcept;

    private:
        void AddCharError(const std::string&, size_t) noexcept;
        void AddDotError(const std::string&, size_t) noexcept;

        std::vector<std::string> errors;
};

}

#endif

