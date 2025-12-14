#pragma once
#ifndef _BpTreeDataNode_H_
#define _BpTreeDataNode_H_

#include "BpTreeNode.h"

#include <string>
#include <vector>

// Data node class for B+ Tree
class BpTreeDataNode : public BpTreeNode
{
private:
	map<string, EmployeeData*> mapData; // Map storing employee data with employee name as key
	BpTreeNode* pNext; // Pointer to the next data node
	BpTreeNode* pPrev; // Pointer to the previous data node

public:
	// Constructor
	BpTreeDataNode()
	{
		pNext = NULL;
		pPrev = NULL;
	}

	// Destructor
	~BpTreeDataNode()
	{
		// Iterate through all entries in the map
		for (auto& i : mapData)
		{
			// Delete each employee data object if it exists
			if (i.second != NULL)
			{
				delete i.second;
			}
		}
		// Clear the map to remove all entries
		mapData.clear();
	}

	// Set the pointer to the next data node
	void setNext(BpTreeNode* pN) { pNext = pN; }

	// Set the pointer to the previous data node
	void setPrev(BpTreeNode* pN) { pPrev = pN; }

	// Get the pointer to the next data node
	BpTreeNode* getNext() { return pNext; }

	// Get the pointer to the previous data node
	BpTreeNode* getPrev() { return pPrev; }

	// Insert a new employee data entry into this node's map
	void insertDataMap(string name, EmployeeData* pN)
	{
		// Insert into map using name as key and employee data as value
		mapData.insert(map<string, EmployeeData*>::value_type(name, pN));
	}

	// Delete an employee data entry from this node's map
	void deleteMap(string name)
	{
		// Erase the entry with the specified name
		mapData.erase(name);
	}

	// Get a pointer to the data map for direct access
	map<string, EmployeeData*>* getDataMap() { return &mapData; }
};

#endif