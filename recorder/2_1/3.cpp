#include <iostream>

using namespace std;

int main() {
	int num = 0;
	char* num_e = new char[1];
	char* temp = nullptr;

	cout << "Enter the number: ";

	while (1)
	{
		char num_imsi = cin.get();

		if (num_imsi == '\n')
			break;

		temp = new char[num + 1];

		for (int i = 0; i < num; i++)
			temp[i] = num_e[i];
		delete[] num_e;
		num_e = temp;

		num_e[num] = num_imsi;

		num++;
	}

	cout << "Reversed number: ";

	for (int i = 0; i < num; i++)
	{
		if (num_e[num - i - 1] != '0')
			cout << num_e[num - i - 1];
	}

	delete[] temp;

	return 0;
}

