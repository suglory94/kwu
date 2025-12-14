#include <iostream>

using namespace std;

int main() {
	int a, b;
	int hap = 0;

	cin >> a >> b;

	int i = 1;
	while (b >= i)
	{
		hap += (i * i);

		b -= i;
		i++;
	}
	hap += b * i;

	int j = 1;
	while (a > j)
	{
		hap -= (j * j);
		
		a -= j;
		j++;
	}
	hap -= (a - 1) * j;

	cout << hap << endl;

	return 0;
}