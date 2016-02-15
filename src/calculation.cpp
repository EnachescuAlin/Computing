#include <stack>
#include <iostream>
#include <cmath>

#include <calculation.h>
#include <lexer.h>

using namespace std;

namespace Computing
{

const map<string, long double (*)(long double)> Calculation::functions = 
{
    /*
     * TODO
     *  To add more functions.
     */
    {"sin", sin},
    {"cos", cos},
    {"tan", tan},
    {"sqrt", sqrt}
};

long double Calculation::Resolve(const string& str) const
    throw(invalid_argument)
{
    if (!CheckParenthesis(str))
        throw invalid_argument("The number of left parenthesis is not equal with"
                               " the number of right parenthesis");

    /*
     * TODO
     *  To test special cases.
     *      5 + * 2
     *      5 + 2 *
     *      * 5 + 2
     *
     *      5 (2 + 3)
     *      (2 + 3) 5
     *
     *      5 2 * 3
     *      5 sin(90)
     *
     *      sin(90) cos(90)
     *      sin 90 cos(45)
     *
     *  To test if there are the functions that are called.
     */



    Lexer l;
    list<Token*> tokens = l.Split(str);
    long double r;

    if (l.PrintErrors())
    {
        throw invalid_argument("Invalid input");
    }

    if (tokens.size() == 0)
    {
        throw invalid_argument("No tokens");
    }

    AddZero(tokens);

    try
    {
        r = Resolve(tokens);

        for (auto it = tokens.begin(); it != tokens.end(); ++it)
            delete (*it);

        return r;
    }
    catch (invalid_argument& e)
    {
        for (auto it = tokens.begin(); it != tokens.end(); ++it)
            delete (*it);

        throw e;
    }
}

bool Calculation::CheckParenthesis(const string& str) const
{
    int difference = 0;
    for (string::size_type i = 0; i < str.size(); i++)
        if (str[i] == '(')
            difference++;
        else
        {
            if (str[i] == ')')
            {
                difference--;
                if (difference == -1)
                {
                    string error = "Error(";
                    error += to_string(i + 1);
                    error += "): ";
                    size_t pos = i + error.size();
                    error += str;
                    error += "\n";
                    error.append(pos, ' ');
                    error += "^\n";
                    error.append(20 > pos ? 0 : pos - 20, ' ');
                    error += "Doesn't exist left paren for this paren\n";
                    cout << error;

                    return false;
                }
            }
        }

    if (difference > 0)
    {
        string::size_type i = str.size() - 1;
        while (difference > 0)
        {
            for (; i >= 0; --i)
            {
                if (str[i] == '(')
                {
                    string error = "Error(";
                    error += to_string(i + 1);
                    error += "): ";
                    size_t pos = i + error.size();
                    error += str;
                    error += "\n";
                    error.append(pos, ' ');
                    error += "^\n";
                    error.append(20 > pos ? 0 : pos - 20, ' ');
                    error += "Doesn't exist right paren for this paren\n";
                    cout << error;
                    --i;
                    break;
                }
            }
            difference--;
        }

        return false;
    }

    return true;
}

void Calculation::AddZero(list<Token*>& tokens) const
{
    list<Token*>::iterator i = tokens.begin(), j;
    if ((*i)->GetType() == TOK_add || (*i)->GetType() == TOK_min)
    {
        tokens.insert(i, new Token((long double) 0.0));
        ++i;
    }

    for (; i != tokens.end(); ++i)
        if ((*i)->GetType() == TOK_add || (*i)->GetType() == TOK_min)
        {
            j = i;
            --j;
            if ((*j)->GetType() == TOK_leftParen)
            {
                tokens.insert(i, new Token((long double) 0.0));
                ++i;
            }
        }
}

bool Calculation::PopOperands(stack<long double>& operands,
                              long double& op1, long double& op2) const
{
    if (!operands.empty())
    {
        op2 = operands.top();
        operands.pop();
    }
    else
        return false;

    if (!operands.empty())
    {
        op1 = operands.top();
        operands.pop();
    }
    else
        return false;

    return true;
}

long double Calculation::Resolve(const list<Token*>& tokens) const
    throw(invalid_argument)
{
    stack<Token*> operators;
    stack<long double> operands;
    Token *_operator;
    long double operand1, operand2;
    bool error;

    for (list<Token*>::const_iterator i = tokens.begin(); i != tokens.end(); ++i)
    {
        if ((*i)->GetType() == TOK_number)
        {
            operands.push((*i)->GetNumber());
            continue;
        }

        if ((*i)->GetType() == TOK_leftParen)
        {
            operators.push(*i);
            continue;
        }

        if ((*i)->GetType() == TOK_rightParen)
        {
            error = true;
            while (!operators.empty())
            {
                _operator = operators.top();
                operators.pop();
                if (_operator->GetType() == TOK_leftParen)
                {
                    error = false;
                    break;
                }

                if (PopOperands(operands, operand1, operand2))
                {
                    try
                    {
                        operands.push(ApplyOperator(_operator,
                                                    operand1, operand2));
                    }
                    catch (invalid_argument& e)
                    {
                        throw e;
                    }
                }
                else
                {
                    throw invalid_argument("Too few operands");
                }
            }

            if (error)
                throw invalid_argument("The left parenthesis was not found");

            if (!operators.empty())
            {
                if (operators.top()->GetType() == TOK_func)
                {
                    long double value = operands.top();
                    operands.pop();
                    operands.push(CallFunction(operators.top(), value));
                    operators.pop();
                }
            }

            continue;
        }

        if ((*i)->IsOperator())
        {
            if (operators.empty())
            {
                operators.push(*i);
                continue;
            }

            if (operators.top()->GetType() == TOK_leftParen)
            {
                operators.push(*i);
                continue;
            }

            try
            {
                while (!operators.empty())
                {
                    if (operators.top()->GetType() == TOK_leftParen)
                        break;
                    if (CompareOperators(operators.top(), *i))
                    {
                        if (PopOperands(operands, operand1, operand2))
                        {
                            operands.push(ApplyOperator(operators.top(),
                                                        operand1, operand2));
                        }
                        else
                            throw invalid_argument("Too few operands");
                        operators.pop();
                    }
                    else
                        break;
                }
                operators.push(*i);
            }
            catch (invalid_argument& e)
            {
                throw e;
            }

            continue;
        }

        if ((*i)->GetType() == TOK_func)
        {
            operators.push(*i);
            continue;
        }

        throw invalid_argument("Unknown token");
    }

    while (!operators.empty())
    {
        if (PopOperands(operands, operand1, operand2))
        {
            try
            {
                operands.push(ApplyOperator(operators.top(),
                                            operand1, operand2));
            }
            catch (invalid_argument& e)
            {
                throw e;
            }

            operators.pop();
        }
        else
        {
            throw invalid_argument("Too few operands");
        }
    }

    if (operands.size() == 0)
        throw invalid_argument("Something's wrong");
    if (operands.size() > 1)
        throw invalid_argument("The values stack are at least two operands and"
                               " operators stack is empty");
    else
        return operands.top();
}

long double Calculation::ApplyOperator(const Token* const& _operator,
                                       const long double& operand1,
                                       const long double& operand2) const
    throw(invalid_argument)
{
    switch (_operator->GetType())
    {
        case TOK_add:
            return (operand1 + operand2);
        case TOK_min:
            return (operand1 - operand2);
        case TOK_mul:
            return (operand1 * operand2);
        case TOK_div:
            return (operand1 / operand2);
        case TOK_pow:
            return pow(operand1, operand2);
        default:
            throw invalid_argument("Unknown operator");
    }
}

bool Calculation::CompareOperators(const Token* const& operator1,
                                   const Token* const& operator2) const
    throw(invalid_argument)
{
    int priority1, priority2;

    switch (operator1->GetType())
    {
        case TOK_add:
        case TOK_min:
            priority1 = 1;
            break;
        case TOK_mul:
        case TOK_div:
            priority1 = 2;
            break;
        case TOK_pow:
            priority1 = 3;
            break;
        default:
            throw invalid_argument("Unknown operator");
    }

    switch (operator2->GetType())
    {
        case TOK_add:
        case TOK_min:
            priority2 = 1;
            break;
        case TOK_mul:
        case TOK_div:
            priority2 = 2;
            break;
        case TOK_pow:
            priority2 = 3;
            break;
        default:
            throw invalid_argument("Unknown operator");
    }

    if (priority1 >= priority2)
        return true;

    return false;
}

long double Calculation::CallFunction(const Token* const& func,
                                      const long double& value) const
    throw(invalid_argument)
{
    if (func == NULL)
        throw invalid_argument("Token is null");

    if (func->GetType() == TOK_func)
    {
        if (functions.count(*(func->GetFunction())) > 0)
        {
            return functions.find(*(func->GetFunction()))->second(value);
        }
        else
        {
            throw invalid_argument("Unknown function");
        }
    }

    throw invalid_argument("Token is not function");
}

}

