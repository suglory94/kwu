#pragma once
#include "EmployeeHeap.h"
#include <algorithm>

// Insert a new employee data into the max heap
void EmployeeHeap::Insert(EmployeeData* data) {
	// Check if data is valid
	if (data == NULL)
	{
		return;
	}

	// If heap is full, resize the array to accommodate more elements
	if (datanum >= maxCapacity)
	{
		ResizeArray();
	}

	// Insert the new data at the end of the heap
	heapArr[++datanum] = data;

	// Restore heap property
	UpHeap(datanum);
}

// Return the top element
EmployeeData* EmployeeHeap::Top() {
	// Return the root element
	return heapArr[1];
}

// Delete the top element
void EmployeeHeap::Delete() {
	// Check if heap is empty
	if (IsEmpty())
	{
		return;
	}

	// Replace root with the last element
	heapArr[1] = heapArr[datanum];
	// Clear the last position
	heapArr[datanum] = NULL;
	// Decrease the number of elements
	datanum--;

	// If heap still has elements, restore heap property
	if (datanum > 0)
	{
		DownHeap(1);
	}
}

// Check if the heap is empty
bool EmployeeHeap::IsEmpty() {
	// Heap is empty if number of elements is 0
	return datanum == 0;
}

// Restore heap property by moving element up
void EmployeeHeap::UpHeap(int index) {
	// If at root or invalid index
	if (index <= 1)
	{
		return;
	}

	// Calculate parent index
	int parent = index / 2;

	// Compare current node with parent
	if (heapArr[parent]->getIncome() < heapArr[index]->getIncome() || 
		(heapArr[parent]->getIncome() == heapArr[index]->getIncome() && heapArr[parent]->getName() > heapArr[index]->getName()))
	{
		// Swap parent and current node
		EmployeeData* temp = heapArr[parent];
		heapArr[parent] = heapArr[index];
		heapArr[index] = temp;

		// Recursively continue up-heap
		UpHeap(parent);
	}
}

// Restore heap property by moving element down
void EmployeeHeap::DownHeap(int index) {
	// If node has no children
	if (index * 2 > datanum)
	{
		return;
	}

	// Calculate children indices
	int leftChild = 2 * index;
	int rightChild = 2 * index + 1;
	// Track the index of the largest element among parent and children
	int tempIndex = index;

	// Check if left child exists and compare with current largest
	if (leftChild <= datanum)
	{
		// Income is higher or incomes equal but name comes before
		bool leftBigger = (heapArr[leftChild]->getIncome() > heapArr[tempIndex]->getIncome()) ||
			(heapArr[leftChild]->getIncome() == heapArr[tempIndex]->getIncome() && heapArr[leftChild]->getName() < heapArr[tempIndex]->getName());

		if (leftBigger)
		{
			tempIndex = leftChild;
		}
	}

	// Check if right child exists and compare with current largest
	if (rightChild <= datanum)
	{
		// Income is higher or incomes equal but name comes before
		bool rightBigger = (heapArr[rightChild]->getIncome() > heapArr[tempIndex]->getIncome()) ||
			(heapArr[rightChild]->getIncome() == heapArr[tempIndex]->getIncome() && heapArr[rightChild]->getName() < heapArr[tempIndex]->getName());

		if (rightBigger)
		{
			tempIndex = rightChild;
		}
	}

	// If a child is larger than parent, swap and continue down-heap
	if (tempIndex != index)
	{
		// Swap parent with the larger child
		EmployeeData* temp = heapArr[index];
		heapArr[index] = heapArr[tempIndex];
		heapArr[tempIndex] = temp;

		// Recursively continue down-heap
		DownHeap(tempIndex);
	}
}

// Resize the heap array when capacity is exceeded
void EmployeeHeap::ResizeArray() {
	// Double the capacity
	int newCapacity = maxCapacity * 2;
	// Allocate new array
	EmployeeData** newArray = new EmployeeData * [newCapacity + 1];

	// Copy existing elements to the new array
	for (int i = 0; i <= maxCapacity; i++)
	{
		newArray[i] = heapArr[i];
	}

	// Initialize new positions to NULL
	for (int i = maxCapacity + 1; i <= newCapacity; i++)
	{
		newArray[i] = NULL;
	}

	// Delete the old array
	delete[] heapArr;
	
	// Update heap array pointer to new array
	heapArr = newArray;
	// Update max capacity
	maxCapacity = newCapacity;
}