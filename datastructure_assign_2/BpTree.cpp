#include "BpTree.h"
#include <vector>
#include <cmath>

// Insert a new employee data into the B+ tree
bool BpTree::Insert(EmployeeData* newData)
{
    // If tree is empty, create a new root data node
    if (root == NULL)
    {
        root = new BpTreeDataNode();
        // Insert the data into the root node
        root->insertDataMap(newData->getName(), newData);
        return true;
    }

    // Find the appropriate data node where this data should be inserted
    BpTreeNode* targetNode = searchDataNode(newData->getName());
    if (targetNode == NULL) return false;

    // Insert the new data into the target node
    targetNode->insertDataMap(newData->getName(), newData);

    // Check if the node has exceeded capacity and needs to be split
    if (excessDataNode(targetNode))
    {
        splitDataNode(targetNode);
    }

    return true;
}

// Check if a data node has exceeded the maximum order
bool BpTree::excessDataNode(BpTreeNode* pDataNode)
{
    // Cast the node to a data node type
    BpTreeDataNode* dataNode = dynamic_cast<BpTreeDataNode*>(pDataNode);
    
    // Return true if the number of entries is greater than or equal to order
    return dataNode->getDataMap()->size() >= order;
}

// Check if an index node has exceeded the maximum order
bool BpTree::excessIndexNode(BpTreeNode* pIndexNode)
{
    // Cast the node to an index node type
    BpTreeIndexNode* indexNode = dynamic_cast<BpTreeIndexNode*>(pIndexNode);

    // Return true if the number of index entries is greater than or equal to order
    return indexNode->getIndexMap()->size() >= order;
}

// Split a data node that has exceeded capacity
void BpTree::splitDataNode(BpTreeNode* pDataNode)
{
    // Cast to data node and create a new data node
    BpTreeDataNode* previousNode = dynamic_cast<BpTreeDataNode*>(pDataNode);
    BpTreeDataNode* newNode = new BpTreeDataNode();
    
    // Get the data map and convert to vector
    map<string, EmployeeData*>* dataMap = previousNode->getDataMap();
    vector<pair<const string, EmployeeData*>> items((*dataMap).begin(), (*dataMap).end());

    // Calculate the split point
    int size = dataMap->size();
    int splitPoint = ceil((order - 1) / 2.0) + 1;
    // The key that will be the parent
    string upKey = items[splitPoint - 1].first;

    // Move the second half of data to the new node
    for (int i = splitPoint - 1; i < size; i++)
    {
        newNode->insertDataMap(items[i].first, items[i].second);
    }

    // Remove the moved data from the original node
    for (int i = splitPoint - 1; i < size; i++)
    {
        dataMap->erase(items[i].first);
    }
    
    // Update the linked list pointers
    newNode->setNext(previousNode->getNext());
    newNode->setPrev(previousNode);
    // If there is a next node, update its previous pointer
    if (previousNode->getNext() != NULL)
    {
        previousNode->getNext()->setPrev(newNode);
    }
    previousNode->setNext(newNode);

    // If splitting the root, create a new root index node
    if (pDataNode == root)
    {
        BpTreeIndexNode* newRoot = new BpTreeIndexNode();
        // Set the left child to the original node
        newRoot->setMostLeftChild(previousNode);
        // Insert the promoted key with the new node as its child
        newRoot->insertIndexMap(upKey, newNode);

        // Update parent pointers
        previousNode->setParent(newRoot);
        newNode->setParent(newRoot);

        // Update the root pointer
        root = newRoot;
    }
    else
    {
        // If not root, promote the key to the parent index node
        BpTreeIndexNode* parentNode = dynamic_cast<BpTreeIndexNode*>(previousNode->getParent());
        if (parentNode != NULL)
        {
            // Set the parent for the new node
            newNode->setParent(parentNode);
            // Insert the promoted key into the parent
            parentNode->insertIndexMap(upKey, newNode);

            // Check if parent needs to be split
            if (excessIndexNode(parentNode))
            {
                splitIndexNode(parentNode);
            }
        }
    }
}

// Split an index node that has exceeded capacitiy
void BpTree::splitIndexNode(BpTreeNode* pIndexNode)
{
    // Cast to index node and create a new index node
    BpTreeIndexNode* previousNode = dynamic_cast<BpTreeIndexNode*>(pIndexNode);
    BpTreeIndexNode* newNode = new BpTreeIndexNode();
    
    // Get the index map and convert to vector
    map<string, BpTreeNode*>* indexMap = previousNode->getIndexMap();
    vector<pair<const string, BpTreeNode*>> items((*indexMap).begin(), (*indexMap).end());

    // Calculate the split point
    int size = indexMap->size();
    int splitPoint = ceil((order - 1) / 2.0) + 1;
    // The key that will be promoted to the parent
    string upKey = items[splitPoint - 1].first;

    // Set the leftmost child of the new node
    newNode->setMostLeftChild(items[splitPoint - 1].second);
    items[splitPoint - 1].second->setParent(newNode);

    // Move entries after the split point to the new node
    for (int i = splitPoint; i < size; i++)
    {
        newNode->insertIndexMap(items[i].first, items[i].second);
        // Update parent pointers
        items[i].second->setParent(newNode);
    }

    // Remove moved entries from the original node
    for (int i = splitPoint - 1; i < size; i++)
    {
        indexMap->erase(items[i].first);
    }

    // If splitting the root, create a new root
    if (pIndexNode == root)
    {
        BpTreeIndexNode* newRoot = new BpTreeIndexNode();
        // Original node becomes left child
        newRoot->setMostLeftChild(previousNode);
        // Promoted key with new node as right child
        newRoot->insertIndexMap(upKey, newNode);

        // Update parent pointers
        previousNode->setParent(newRoot);
        newNode->setParent(newRoot);

        // Update root pointer
        root = newRoot;
    }
    else
    {
        // If not root, promote key to parent
        BpTreeIndexNode* parentNode = dynamic_cast<BpTreeIndexNode*>(previousNode->getParent());
        if (parentNode != NULL)
        {
            // Set parent for new node
            newNode->setParent(parentNode);
            // Insert promoted key into parent
            parentNode->insertIndexMap(upKey, newNode);

            // Recursively split parent if needed
            if (excessIndexNode(parentNode))
            {
                splitIndexNode(parentNode);
            }
        }
    }
}

// Search for the data node that should contain the given name
BpTreeNode* BpTree::searchDataNode(string name)
{
    // Return NULL if tree is empty
    if (root == NULL)
    {
        return NULL;
    }

    // Start from root and traverse down
    BpTreeNode* current = root;
    // Continue until we reach a data node
    while (dynamic_cast<BpTreeDataNode*>(current) == NULL)
    {
        // Cast to index node
        BpTreeIndexNode* indexNode = dynamic_cast<BpTreeIndexNode*>(current);
        if (indexNode == NULL)
        {
            return NULL;
        }
        
        // Get the index map of the current node
        map<string, BpTreeNode*>* indexMap = indexNode->getIndexMap();
        // Start with the leftmost child
        BpTreeNode* nextNode = current->getMostLeftChild();

        // Find the approporiate child to descend to
        for (auto& i : *indexMap)
        {
            // If the search key is less than current key, use previous child
            if (name < i.first)
            {
                break;
            }
            // Otherwise, use this child
            nextNode = i.second;
        }
        // Move to the next level
        current = nextNode;
        if (current == NULL)
        {
            return NULL;
        }
    }
    // Return the data node found
    return current;
}

// Search for any data within the given range
bool BpTree::searchRange(string start, string end)
{
    // Return false if tree is empty
    if (root == NULL)
    {
        return false;
    }

    // Find the data node where start key should be
    BpTreeNode* node = searchDataNode(start);
    if (node == NULL)
    {
        return false;
    }

    // Cast to data node
    BpTreeDataNode* dataNode = dynamic_cast<BpTreeDataNode*>(node);
    if (dataNode == NULL)
    {
        return false;
    }

    // Traverse through data nodes using the linked list
    while (dataNode != NULL)
    {
        // Get the data map of current node
        map<string, EmployeeData*>* dataMap = dataNode->getDataMap();

        // Check each entry in the node
        for (auto& i : *dataMap)
        {
            const string& key = i.first;

            // Skip keys before the start of range
            if (key < start)
            {
                continue;
            }
            // No match found
            if (key > end)
            {
                return false;
            }
            // Found a key within the range
            return true;
        }
        // Move to next data node in the linked list
        dataNode = dynamic_cast<BpTreeDataNode*>(dataNode->getNext());
    }
    // No data found
    return false;
}

// Check if the tree is empty
bool BpTree::isEmpty()
{
    // Tree is empty if root is NULL
    if (root == NULL)
    {
        return true;
    }
    return false;
}

// Recursively delete all nodes in the tree
void BpTree::deleteTree(BpTreeNode* node)
{
    // NULL node
    if (node == NULL)
    {
        return;
    }

    // Check if this is an index node
    BpTreeIndexNode* indexNode = dynamic_cast<BpTreeIndexNode*>(node);
    if (indexNode != NULL)
    {
        // Recursively delete the leftmost child subtree
        deleteTree(node->getMostLeftChild());
        
        // Get the index map
        map<string, BpTreeNode*>* indexMap = indexNode->getIndexMap();
        // Recursively delete all other child subtrees
        for (auto& i : *indexMap)
        {
            deleteTree(i.second);
        }
    }
    // Delete the current node after deleting all children
    delete node;
}