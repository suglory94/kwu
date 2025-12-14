#include <iostream>

using namespace std;

int pow(int exponent, int power);
void hadamardMatrix(int** matrix, int size, int x, int y, int value);

int main() {
	int n;

	cout << "Enter the value of n for Hadamard matrix (2^n x 2^n): ";
	cin >> n;

	int size = pow(2, n);

	cout << "\nHadamard Matrix of size " << size << "x" << size << endl;
	
	int** matrix = new int*[size];
	for (int i = 0; i < size; i++)
		matrix[i] = new int[size];

	hadamardMatrix(matrix, size, 0, 0, 1);

	// hadamard matrix 출력
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
			cout << matrix[i][j] << "\t";
		cout << endl;
	}

	// 메모리 해제
	for (int i = 0; i < size; i++)
		delete[] matrix[i];
	delete[] matrix;

	return 0;
}

/* 거듭제곱 함수
* base : 밑
* exponent : 지수 */
int pow(int base, int exponent) {
	if (exponent != 0)
		return base * pow(base, exponent - 1);
	else
		return 1;
}

// 재귀적으로 hadamard matrix 생성
void hadamardMatrix(int** matrix, int size, int x, int y, int value) {
	if (size == 1) // 종료 조건 : 크기가 1일 때
	{
		matrix[x][y] = value;
		return;
	}
	else
	{
		int newSize = size / 2; // 4등분 후, 재참조

		hadamardMatrix(matrix, newSize, x, y, value);
		hadamardMatrix(matrix, newSize, x + newSize, y, value);
		hadamardMatrix(matrix, newSize, x, y + newSize, value);
		hadamardMatrix(matrix, newSize, x + newSize, y + newSize, -value);
	}
}


