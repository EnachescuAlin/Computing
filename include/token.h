#ifndef COMPUTING_TOKEN
#define COMPUTING_TOKEN

#include <string>

namespace Computing
{

enum TOKEN
{
    TOK_min,
    TOK_add,
    TOK_mul,
    TOK_div,
    TOK_pow,
    TOK_func,
    TOK_leftParen,
    TOK_rightParen,
    TOK_number
};

class Token
{
    public:
        Token(const std::string&) noexcept;
        Token(long double) noexcept;
        Token(char) noexcept;

        virtual~ Token(void) noexcept;

        const TOKEN&       GetType(void) const noexcept;
        const std::string* GetFunction(void) const noexcept;
        const long double& GetNumber(void) const noexcept;
        const char&        GetOperator(void) const noexcept;
        const char&        GetParen(void) const noexcept;
        bool IsOperator(void) const noexcept;

        void Print(void) const noexcept;

    private:
        union
        {
            std::string *_function;
            long double  _number;
            char         _operator;
            char         _paren;
        };
        TOKEN tok;
};

}

#endif

