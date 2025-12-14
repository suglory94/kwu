#pragma once
#ifndef _BPTREEINDEXNODE_H_
#define _BPTREEINDEXNODE_H_

#include "BpTreeNode.h"
#include <map>

// Index node class for B+ tree
class BpTreeIndexNode : public BpTreeNode
{
private:
	// Map storing index entries
	map<string, BpTreeNode*> mapIndex;

public:
	BpTreeIndexNode() {} // Constructor
	~BpTreeIndexNode() {} // Destructor

	// Insert a new index entry into this node's map
	void insertIndexMap(string name, BpTreeNode* pN)
	{
		// Insert into map using name as key and child node pointer as value
		mapIndex.insert(map<string, BpTreeNode*>::value_type(name, pN));
	}

	// Delete an index entry from this node's map
	void deleteMap(string name)
	{
		// Erase the entry with the specified name
		mapIndex.erase(name);
	}

	// Get a pointer to the index map for direct access
	map<string, BpTreeNode*>* getIndexMap() { return &mapIndex; }
};

#endif