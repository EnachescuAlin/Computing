#include <token.h>

#include <iostream>

using namespace std;

namespace Computing
{

Token::Token(const string& str) noexcept
{
    _function = new string(str);
    tok = TOK_func;
}

Token::Token(long double number) noexcept
{
    _number = number;
    tok = TOK_number;
}

Token::Token(char ch) noexcept
{
    switch (ch)
    {
        case '-':
            _operator = ch;
            tok = TOK_min;
            break;
        case '*':
            _operator = ch;
            tok = TOK_mul;
            break;
        case '/':
            _operator = ch;
            tok = TOK_div;
            break;
        case '+':
            _operator = ch;
            tok = TOK_add;
            break;
        case '^':
            _operator = ch;
            tok = TOK_pow;
            break;
        case '(':
            _paren = ch;
            tok = TOK_leftParen;
            break;
        case ')':
            _paren = ch;
            tok = TOK_rightParen;
            break;
    }
}

Token::~Token(void) noexcept
{
    if (tok == TOK_func)
        delete _function;
}

const TOKEN& Token::GetType(void) const noexcept
{
    return tok;
}

const string* Token::GetFunction(void) const noexcept
{
    return _function;
}

const long double& Token::GetNumber(void) const noexcept
{
    return _number;
}

const char& Token::GetOperator(void) const noexcept
{
    return _operator;
}

const char& Token::GetParen(void) const noexcept
{
    return _paren;
}

bool Token::IsOperator(void) const noexcept
{
    return (tok == TOK_min || tok == TOK_add || tok == TOK_div ||
            tok == TOK_mul || tok == TOK_pow);
}

void Token::Print(void) const noexcept
{
    switch (tok)
    {
        case TOK_min:
        case TOK_add:
        case TOK_mul:
        case TOK_div:
        case TOK_pow:
            cout << _operator;
            break;
        case TOK_leftParen:
        case TOK_rightParen:
            cout << _paren;
            break;
        case TOK_func:
            cout << *_function;
            break;
        case TOK_number:
            cout << _number;
            break;
        default:
            cout << "ERROR\n";
    }
}

}

