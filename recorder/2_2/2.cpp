#include <iostream>

using namespace std;

int main() {
    int a = 0;
    int b = 0;
    int v = 0;
    int date = 0;

    cin >> a;
    cin >> b;
    cin >> v;

    if (1 <= b < a <= v <= 1000000000)
    {
        if ((v - b) % (a - b) == 0)
            date = (v - b) / (a - b);
        else
            date = (v - b) / (a - b) + 1;
    }

    cout << date;

    return 0;
}