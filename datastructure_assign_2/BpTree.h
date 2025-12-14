#pragma once
#ifndef _BpTree_H_
#define _BpTree_H_

#include "BpTreeDataNode.h"
#include "BpTreeIndexNode.h"
#include "EmployeeData.h"
#include "SelectionTree.h"

// B+ Tree class
class BpTree
{
private:
	BpTreeNode* root; // Pointer to the root node
	int order; // Order of the B+ tree

public:
	// Constructor
	BpTree(ofstream* fout, int order = 3)
	{
		root = NULL;
		this->order = order;
	}

	// Destructor
	~BpTree()
	{
		deleteTree(root); // Recursively delete all nodes
	}

	// Insert a new employee data
	bool Insert(EmployeeData* newData);

	// Check if a data node has exceeded its capacity
	bool excessDataNode(BpTreeNode* pDataNode);

	// Check if an index node has exceeded its capacity
	bool excessIndexNode(BpTreeNode* pIndexNode);

	// Split a data node that has exceeded capacity into two nodes
	void splitDataNode(BpTreeNode* pDataNode);

	// Split an index node that has exceeded capacity into two nodes
	void splitIndexNode(BpTreeNode* pIndexNode);

	// Get the root node of the B+ tree
	BpTreeNode* getRoot() { return root; }

	// Search for the leaf node
	BpTreeNode* searchDataNode(string name);

	// Search if there exists any employee name within the given range
	bool searchRange(string start, string end);

	// Check if the B+ tree is empty
	bool isEmpty();

	// Recursively delete all nodes in the tree
	void deleteTree(BpTreeNode* node);
};

#endif
