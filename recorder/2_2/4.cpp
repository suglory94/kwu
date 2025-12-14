#include <iostream>

using namespace std;

int fibonacci_iter(int n);
int fibonacci_rec(int n);

int main() {
	int n = 0;

	cout << "Enter the number : ";
	cin >> n;

	cout << "Fibonacci_iter(" << n << ") : " << fibonacci_iter(n) << endl;
	cout << "Fibonacci_rec(" << n << ") : " << fibonacci_rec(n) << endl;

	return 0;
}

int fibonacci_iter(int n) {
	if (n <= 2)
		return n - 1;

	int result = fibonacci_iter(n - 1) + fibonacci_iter(n - 2);

	return result;
}

int fibonacci_rec(int n) {
	int* result = new int[n];

	result[0] = 0;
	result[1] = 1;

	for (int i = 2; i < n; i++)
		result[i] = result[i - 1] + result[i - 2];

	return result[n - 1];
}
