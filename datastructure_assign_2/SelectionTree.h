#pragma once
#include "SelectionTreeNode.h"

// Class representing a Selection Tree
class SelectionTree {
private:
    SelectionTreeNode* root; // Pointer to the root node
    ofstream* fout; // Pointer to the output file stream
    SelectionTreeNode* run[8]; // Array of pointers to the 8 leaf nodes

public:
    // Constructor
    SelectionTree(ofstream* fout) {
        this->fout = fout;
        this->root = nullptr;

        // Initialize run array pointers to nullptr
        for (int i = 0; i < 8; i++)
        {
            run[i] = nullptr;
        }
        setTree(); // Build the initial tree structure
    }

    // Destructor
    ~SelectionTree() {
        if (root)
        {
            deleteTree(root); // Recursively deletes all nodes starting from the root
        }
    }

    // Setter for the root node
    void setRoot(SelectionTreeNode* pN) { this->root = pN; }
    
    // Getter for the root node
    SelectionTreeNode* getRoot() { return root; }

    // Builds the initial tree structure
    void setTree();

    // Inserts a new employee into the appropriate department's heap
    bool Insert(EmployeeData* newData);

    // Deletes the overall winner
    bool Delete();

    // Prints all employee data in the heap
    bool printEmployeeData(int dept_no);

    // Gets the pointer to the leaf node
    SelectionTreeNode* getRunNode(int index);

    // Recursively deletes the entire tree structure
    void deleteTree(SelectionTreeNode* node);

    // Updates the winner path from a leaf node up to the root after an insertion or deletion
    void updateTree(SelectionTreeNode* node);
};