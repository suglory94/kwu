#pragma once
#include "EmployeeData.h"

// Base class for all B+ Tree nodes
class BpTreeNode {
private:
	BpTreeNode* pParent; // Pointer to the parent node
	BpTreeNode* pMostLeftChild; // Pointer to the most left child

public:
	// Constructor
	BpTreeNode() {
		pParent = NULL;
		pMostLeftChild = NULL;
	}
	// Destructor
	~BpTreeNode() {

	}

	// Setter for the most left child pointer
	void setMostLeftChild(BpTreeNode* pN) { pMostLeftChild = pN; }
	
	// Setter for the parent pointer
	void setParent(BpTreeNode* pN) { pParent = pN; }

	// Getter for the parent pointer
	BpTreeNode* getParent() { return pParent; }
	
	// Getter for the most left child pointer
	BpTreeNode* getMostLeftChild() { return pMostLeftChild; }

	// Set the next data node
	virtual void setNext(BpTreeNode* pN) {}
	// Set the previous data node
	virtual void setPrev(BpTreeNode* pN) {}
	// Get the next data node
	virtual BpTreeNode* getNext() { return NULL; }
	// Get the previous data node
	virtual BpTreeNode* getPrev() { return NULL; }

	// Insert data into the map
	virtual void insertDataMap(string n, EmployeeData* pN) {}
	// Insert index into the map
	virtual void insertIndexMap(string n, BpTreeNode* pN) {}
	// Delete an entry from the map
	virtual void deleteMap(string n) {}

	// Get the index map pointer
	virtual map<string, BpTreeNode*>* getIndexMap() { return {}; }
	// Get the data map pointer
	virtual map<string, EmployeeData*>* getDataMap() { return {}; }

};
