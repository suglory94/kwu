#include <iostream>

using namespace std;

void quickSort(int* num, int begin, int end);
void merge(int* num, int begin, int mid, int end);
void mergeSort(int* num, int begin, int end);
int binarySearch(int* num, int begin, int end, int search);

int main() {
	int* num = new int[11];
	int method, search;

	for (int i = 0; i < 10; i++)
		num[i] = rand() % 101;

	cout << "Random values : ";
	for (int i = 0; i < 10; i++)
		cout << " " << num[i];
	cout << endl;

	cout << "Select sorting method (1: Quick Sort, 2: Merge Sort) : ";
	cin >> method;

	if (method == 1)
	{
		quickSort(num, 0, 9);
		cout << "Sorted numbers (Quick Sort):";
	}
	else if (method == 2)
	{
		mergeSort(num, 0, 9);
		cout << "Sorted numbers (Merge Sort):";
	}

	for (int i = 0; i < 10; i++)
		cout << " " << num[i];
	cout << endl;

	cout << "Enter a value to search : ";
	cin >> search;

	int result = binarySearch(num, 0, 9, search);
	if (result == 0)
	{
		cout << "Updated numbers : ";

		for (int i = 0; i < 11; i++)
			cout << " " << num[i];
		cout << endl;
	}
	else
		cout << "Searched number index : " << result << endl;

	delete[] num;

	return 0;
}

void quickSort(int* num, int begin, int end) {
	if (begin >= end)
		return;

	int pivot = begin;

	int i = begin + 1;
	int j = end;
	
	while (i <= j)
	{
		while (num[i] <= num[pivot])
			i++;
		while ((num[j] >= num[pivot]) && (j > begin))
			j--;
		if (i > j)
		{
			int tmp = num[j];
			num[j] = num[pivot];
			num[pivot] = tmp;
		}
		else
		{
			int tmp = num[i];
			num[i] = num[j];
			num[j] = tmp;
		}

		quickSort(num, begin, j - 1);
		quickSort(num, j + 1, end);
	}
}

void merge(int* num, int begin, int mid, int end) {
	int* sorted = new int[11];

	int i = begin;
	int j = mid + 1;
	int k = begin;

	while (i <= mid && j <= end)
	{
		if (num[i] <= num[j])
			sorted[k++] = num[i++];
		else
			sorted[k++] = num[j++];
	}

	int tmp = i > mid ? j : i;

	while (k <= end)
		sorted[k++] = num[tmp++];

	for (int i = begin; i <= end; i++)
		num[i] = sorted[i];

	delete[] sorted;
}

void mergeSort(int* num, int begin, int end) {
	int mid;

	if (begin < end)
	{
		mid = (begin + end) / 2;
		mergeSort(num, begin, mid);
		mergeSort(num, mid + 1, end);
		merge(num, begin, mid, end);
	}
}

int binarySearch(int* num, int begin, int end, int search) {

	while (begin <= end)
	{
		int mid = (begin + end) / 2;

		if (num[mid] == search)
			return mid;
		else if (num[mid] > search)
			end = mid - 1;
		else if (num[mid] < search)
			begin = mid + 1;
	}
	return 0;
}