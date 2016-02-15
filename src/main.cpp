#include <iostream>

#include <calculation.h>

using namespace std;
using namespace Computing;

int main(void)
{
    // ((2 + 3) * 5 + 17) / 7 + (((1 + 1) * 5) - 8) + (-3);
    string str;
    getline(cin, str);

    Calculation c;
    try
    {
        cout << c.Resolve(str) << "\n";
    }
    catch (invalid_argument& e)
    {
        cout << e.what() << "\n";
    }

    return 0;
}

