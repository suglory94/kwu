#include <iostream>

using namespace std;

class Calculator {
private:
	enum operations {
		LEFT_PARENTHESIS = '(',
		RIGHT_PARENTHESIS = ')',
		MULTIPHY = '*',
		DIVIDE = '/',
		PLUS = '+',
		MINUS = '-'
	};
public:
	int priority(char op, int )
};

int main() {
	char formulaPart;

	int numbers;
	char operations;

	int cnt_num;
	int cnt_oper;

	cout << "Enter the formula : ";

	while (cnt_oper <= 8) {
		cin >> formulaPart;

		if ((int)'0' <= (int)formulaPart <= (int)'255')
		{
			cnt_num++;
		}
		else
		{
			cnt_oper++;
		}
	}
}