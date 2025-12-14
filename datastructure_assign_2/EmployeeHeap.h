#pragma once

#include "EmployeeData.h"

// Class representing a heap structure for EmployeeData pointers
class EmployeeHeap {
private:
    int datanum; // Current number of data elements
    EmployeeData** heapArr; // Dynamic array to store EmployeeData pointers
    int maxCapacity; // Maximum allocated capacity of the heap array

public:
    // Constructor
    EmployeeHeap(int theMaxCapacity = 10) {
        if (theMaxCapacity < 1)
        {
            throw "Capacity must be >= 1."; //Throw exception
        }

        datanum = 0;
        maxCapacity = theMaxCapacity;
        // Allocate memory for the heap array
        heapArr = new EmployeeData * [maxCapacity + 1];

        for (int i = 0; i <= maxCapacity; i++)
        {
            heapArr[i] = NULL;
        }
    }
    // Destructor
    ~EmployeeHeap() {
        if (heapArr != NULL)
        {
            delete[] heapArr; // Delete the array itself
            heapArr = NULL;
        }
    }

    // Inserts a new employee data pointer into the heap
    void Insert(EmployeeData* data);

    // Returns the data pointer at the root
    EmployeeData* Top();

    // Deletes the root element and restores the heap
    void Delete();

    // Checks if the heap is empty
    bool IsEmpty();

    // Restores heap by moving an element up the tree
    void UpHeap(int index);

    // Restores heap by moving an element down the tree
    void DownHeap(int index);

    // Doubles the size of the internal array
    void ResizeArray();

    // Getter for the current number of data elements
    int getDataNum() { return datanum; }

    // Getter for the heap array pointer
    EmployeeData** getHeapArr() { return heapArr; }
};