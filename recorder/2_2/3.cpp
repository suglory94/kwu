#include <iostream>

using namespace std;

int main() {
	int size = 0;

	cout << "Size of the array : ";
	cin >> size;

	int* num = new int[size];

	cout << "Random numbers : ";

	for (int i = 0; i < size; i++)
		cin >> num[i];

	int max = -1;
	int min = 101;
	int max_num = 0;
	int min_num = 0;

	for (int i = 0; i < size; i++)
	{
		if (num[i] > max)
		{
			max = num[i];
			max_num = i;
		}
		if (num[i] < min)
		{
			min = num[i];
			min_num = i;
		}
	}

	cout << "Maximum value : " << max << ",Address : " << &num[max_num] << endl;
	cout << "Minimum value : " << min << ",Address : " << &num[min_num];

	delete[] num;

	return 0;
}