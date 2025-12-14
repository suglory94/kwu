#include <iostream>

using namespace std;

int main() {
	char str[101] = { 0 };
	int num = 0;
	
	while (num < 100) {
		cin.getline(str, 101);

		if (str[0] == '\0')
			break;

		cout << str << endl;
		
		num++;
	}

	return 0;
}