#include <iostream>

using namespace std;

int main() {
	int num = 0;

	while (1) {
		cout << "Enter the number of rows: ";
		cin >> num;
		
		int num_di = num / 2;

		if ((num % 2 == 0) || (num < 1) || (num > 19))
		{
			cout << "Please enter again." << endl;
			continue;
		}
		else
		{
			for (int i = 0; i < num_di + 1; i++)
			{
				for (int j = 0; j < num; j++)
				{
					if ((j == num_di - i) || (j == num - num_di + i - 1))
						cout << "*";
					else
						cout << " ";
				}
				cout << endl;
			}

			for (int i = num_di - 1; i >= 0 ; i--)
			{
				for (int j = 0; j < num; j++)
				{
					if ((j == num_di - i) || (j == num - num_di + i - 1))
						cout << "*";
					else
						cout << " ";
				}
				cout << endl;
			}
			break;
		}
	}
	return 0;
}