#pragma once
#include "EmployeeData.h"
#include "EmployeeHeap.h"

// Class representing a node in the Selection Tree
class SelectionTreeNode {
private:
    EmployeeData* pData; // Pointer to the winning EmployeeData
    SelectionTreeNode* pLeft; // Pointer to the left child node
    SelectionTreeNode* pRight; // Pointer to the right child node
    SelectionTreeNode* pParent; // Pointer to the parent node
    EmployeeHeap* hRoot; // Pointer to the EmployeeHeap

public:
    // Constructor
    SelectionTreeNode() {
        this->pData = NULL;
        this->pLeft = NULL;
        this->pRight = NULL;
        this->pParent = NULL;
        this->hRoot = NULL;
    }

    // Destructor
    ~SelectionTreeNode() {
        if (hRoot) delete hRoot;
    }

    // Initializes the EmployeeHeap
    void HeapInit() { this->hRoot = new EmployeeHeap; }

    // Setters
    void setEmployeeData(EmployeeData* data) { this->pData = data; }
    void setLeftChild(SelectionTreeNode* pL) { this->pLeft = pL; }
    void setRightChild(SelectionTreeNode* pR) { this->pRight = pR; }
    void setParent(SelectionTreeNode* pP) { this->pParent = pP; }
    void setHeap(EmployeeHeap* pHR) { this->hRoot = pHR; }

    // Getters
    EmployeeData* getEmployeeData() { return pData; }
    SelectionTreeNode* getLeftChild() { return pLeft; }
    SelectionTreeNode* getRightChild() { return pRight; }
    SelectionTreeNode* getParent() { return pParent; }
    EmployeeHeap* getHeap() { return hRoot; }
};