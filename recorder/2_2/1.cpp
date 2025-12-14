#include <iostream>

using namespace std;

int main() {
	int n = 0;
	int m = 0;

	int num1 = 0;
	int num2 = 0;


	cin >> n;
	cin >> m;

	int* arr = new int[n];

	for (int i = 0; i < n; i++)
		arr[i] = i + 1;

	for (int i = 0; i < m; i++)
	{
		cin >> num1;
		cin >> num2;

		int tmp;
		int& a = tmp;

		a = arr[num1 - 1];
		arr[num1 - 1] = arr[num2 - 1];
		arr[num2 - 1] = tmp;
	}

	for (int i = 0; i < n; i++)
		cout << arr[i] << " ";

	delete[] arr;

	return 0;
}