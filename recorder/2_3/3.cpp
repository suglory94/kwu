#include <iostream>

using namespace std;

int main() {
	char* string = new char[100];
	char delimiter;

	cout << "Enter the string : ";
	cin.getline(string, 100);

	cout << "Enter the delimiter : ";
	cin.get(delimiter);
	
	cout << endl << "Separated tokens : " << endl;

	int i = 0;
	while (string[i] != '\0')
	{
		if (string[i] == delimiter)
		{
			cout << endl;
			i++;
			continue;
		}
		cout << string[i];
		i++;
	}

	delete[] string;

	return 0;
}