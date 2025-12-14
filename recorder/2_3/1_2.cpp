#include <iostream>

using namespace std;

int main() {
	int a, b, cnt;
	int hap = 0;

	cin >> a >> b;

	int* num = new int[b];

	for (int i = 1; i <= b; i++)
	{
		num[i - 1] = cnt;
		if (i >= a)
			hap += num[i - 1];
	}

	delete[] num;

	return 0;
}