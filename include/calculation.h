#ifndef COMPUTING_CALCULATION
#define COMPUTING_CALCULATION

#include <string>
#include <list>
#include <stdexcept>
#include <stack>
#include <map>

#include "token.h"

namespace Computing
{

class Calculation
{
    public:
        /*
         * Tries to resolve the calculation sent through string.
         * If it fails then throws an exception.
         */
        long double Resolve(const std::string&) const
            throw(std::invalid_argument);

    private:
        /*
         * Tries to resolve the calculation sent through the list of tokens.
         * If it fails then throws an exception.
         */
        long double Resolve(const std::list<Token*>&) const
            throw(std::invalid_argument);

        /*
         * Adds zero where needed.
         * E.g.:
         *  +3 + (-3) => 0 + 3 + (0 - 3)
         *  -3 + 3    => 0 - 3 + 3
         */
        void AddZero(std::list<Token*>&) const;

        /*
         * Tries to remove two operands from stack.
         * Return true if in stack are at least two operands, otherwise false.
         */
        bool PopOperands(std::stack<long double>&,
                         long double&,
                         long double&) const;

        /*
         * Tries to apply the operator on two operands.
         * If the tokens isn't an operator then throws an exception.
         */
        long double ApplyOperator(const Token* const&,
                                  const long double&,
                                  const long double&) const
            throw(std::invalid_argument);

        /*
         * Tries to compare two operators.
         * If at least one between tokens isn't operator then throws an exception.
         * Return true if the first operator has priority greater or equal than
         * the priority of the second operator, otherwise false.
         */
        bool CompareOperators(const Token* const&, const Token* const&) const
            throw(std::invalid_argument);

        /*
         * Tries to call the function.
         * If the token isn't a function then throws an exception.
         * Return the function value.
         */
        long double CallFunction(const Token* const&, const long double&) const
            throw(std::invalid_argument);

        /*
         * Checks if the number of left parenthesis is equal with the number of
         * right parenthesis.
         */
        bool CheckParenthesis(const std::string&) const;

        typedef long double __func(long double);
        static const std::map<std::string, __func*> functions;
};

}

#endif
