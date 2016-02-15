#include <lexer.h>

#include <iostream>

using namespace std;

namespace Computing
{

bool Lexer::PrintErrors(void) noexcept
{
    if (errors.size() == 0)
        return false;

    for (const auto& i : errors)
        cout << i << "\n";

    errors.clear();

    return true;
}

void Lexer::AddCharError(const string& str, size_t pos) noexcept
{
    string error = "Error (";
    error += to_string(pos + 1);
    error += "): ";

    pos += error.size();

    error += str + '\n';
    error.append(pos, ' ');
    error += "^\n";
    error.append(pos - 6, ' ');
    error += "unknown char\n";

    errors.push_back(error);
}

void Lexer::AddDotError(const string& str, size_t pos) noexcept
{
    string error = "Error (";
    error += to_string(pos + 1);
    error += "): ";

    pos += error.size();

    error += str + '\n';
    error.append(pos, ' ');
    error += "^\n";
    error.append(pos - 12, ' ');
    error += "too many dots in a number\n";

    errors.push_back(error);
}

list<Token*> Lexer::Split(const string& input) noexcept
{
    size_t i = 0, j, size = input.size();
    list<Token*> tokens;
    int dots;

    while (i < size)
    {
        switch (input[i])
        {
            case '(':
            case ')':
            case '^':
            case '/':
            case '*':
            case '-':
            case '+':
                tokens.push_back(new Token(input[i]));
                i++;
                continue;
            case ' ':
            case '\t':
                i++;
                continue;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                j = i + 1;
                dots = 0;
                while (j < size && ((input[j] >= '0' && input[j] <= '9') ||
                                     input[j] == '.'))
                {
                    if (input[j] == '.')
                    {
                        if (dots)
                            AddDotError(input, j);
                        dots++;
                    }
                    j++;
                }
                if (dots > 1)
                    tokens.push_back(new Token((long double) 0));
                else
                    tokens.push_back(new Token(stold(string(input, i, j - i))));

                i = j;
                continue;
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
                j = i + 1;
                while (j < size && ((input[j] >= 'a' && input[j] <= 'z') ||
                                    (input[j] >= 'A' && input[j] <= 'Z') ||
                                    (input[j] >= '0' && input[j] <= '9')))
                    j++;
                tokens.push_back(new Token(string(input, i, j - i)));
                i = j;
                continue;
            default:
                AddCharError(input, i);
                i++;
                continue;
        }
    }

    return tokens;
}

}

