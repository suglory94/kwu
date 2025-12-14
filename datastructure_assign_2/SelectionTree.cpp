#include "SelectionTree.h"
#include <vector>
#include <algorithm>

// Initialize the selection tree structure with 8 leaf runs
void SelectionTree::setTree() {
	// Create 8 leaf nodes
	for (int i = 0; i < 8; i++)
	{
		run[i] = new SelectionTreeNode();
		run[i]->HeapInit(); // Initialize the heap for this run
	}

	// Build level 2
	SelectionTreeNode* level2[4];
	for (int i = 0; i < 4; i++)
	{
		level2[i] = new SelectionTreeNode();
		// Connect to children
		level2[i]->setLeftChild(run[i * 2]);
		level2[i]->setRightChild(run[i * 2 + 1]);
		// Set parent pointers for the children
		run[i * 2]->setParent(level2[i]);
		run[i * 2 + 1]->setParent(level2[i]);
	}

	// Build level 1
	SelectionTreeNode* level1[2];
	for (int i = 0; i < 2; i++)
	{
		level1[i] = new SelectionTreeNode();
		// Connect to children
		level1[i]->setLeftChild(level2[i * 2]);
		level1[i]->setRightChild(level2[i * 2 + 1]);
		// Set parent pointers for the children
		level2[i * 2]->setParent(level1[i]);
		level2[i * 2 + 1]->setParent(level1[i]);
	}

	// Create the root node
	root = new SelectionTreeNode();
	root->setLeftChild(level1[0]);
	root->setRightChild(level1[1]);
	// Set parent pointers
	level1[0]->setParent(root);
	level1[1]->setParent(root);
}

// Insert a new employee data
bool SelectionTree::Insert(EmployeeData* newData) {
	// Calculate which run this employee belongs to
	int index = (newData->getDeptNo() / 100) - 1;

	// Get the leaf node
	SelectionTreeNode* leaf = run[index];

	// Get the heap
	EmployeeHeap* heap = leaf->getHeap();
	if (!heap)
	{
		return false;
	}

	// Insert the new data into the department's heap
	heap->Insert(newData);
	// Update the leaf's employee data to the top of the heap
	leaf->setEmployeeData(heap->Top());
	// Propagate the change up the tree to maintain selection tree property
	updateTree(leaf);
	return true;
}

// Delete the employee with maximum income from the tree
bool SelectionTree::Delete() {
	// Check if tree is valid and has data
	if (!root || !root->getEmployeeData())
	{
		return false;
	}

	// Get the employee data at the root
	EmployeeData* rootData = root->getEmployeeData();
	// Find which department this employee belongs to
	int index = (rootData->getDeptNo() / 100) - 1;

	// Get the corresponding leaf node
	SelectionTreeNode* leaf = run[index];

	// Get the heap for this department
	EmployeeHeap* heap = leaf->getHeap();
	if (!heap || heap->IsEmpty())
	{
		return false;
	}

	// Delete the top element from the heap
	heap->Delete();
	// Update the leaf's employee data to the new top of the heap
	leaf->setEmployeeData(heap->Top());
	// Propagate the change up the tree
	updateTree(leaf);
	return true;
}

// Print all employee data for a specific department
bool SelectionTree::printEmployeeData(int dept_no) {
	// Calculate the index of the run
	int index = (dept_no / 100) - 1;

	// Get the leaf node
	SelectionTreeNode* leaf = run[index];

	// Get the heap
	EmployeeHeap* heap = leaf->getHeap();
	if (!heap || heap->IsEmpty())
	{
		return false;
	}

	// Get the number of elements
	int count = heap->getDataNum();
	// Get the heap array
	EmployeeData** array = heap->getHeapArr();

	// Copy all employee data
	vector<EmployeeData*> v;
	for (int i = 1; i <= count; i++)
	{
		v.push_back(array[i]);
	}

	// Sort the vector by income in descending order
	sort(v.begin(), v.end(), [](EmployeeData* a, EmployeeData* b) { return a->getIncome() > b->getIncome(); });
	return true;
}

// Get a specific run node by index
SelectionTreeNode* SelectionTree::getRunNode(int index) {
	return run[index];
}

// Recursively delete all nodes in the tree
void SelectionTree::deleteTree(SelectionTreeNode* node) {
	// If node is NULL
	if (node == NULL)
	{
		return;
	}

	// Recursively delete left subtree
	deleteTree(node->getLeftChild());
	// Recursively delete right subtree
	deleteTree(node->getRightChild());
	// Delete the current node after deleting its children
	delete node;
}

// Update the selection tree from a leaf node up to the root
void SelectionTree::updateTree(SelectionTreeNode* leaf) {
	// Start from the leaf's parent and move up to the root
	SelectionTreeNode* current = leaf->getParent();

	// Continue until we reach the root
	while (current)
	{
		// Get the employee data from both children
		EmployeeData* leftData = current->getLeftChild()->getEmployeeData();
		EmployeeData* rightData = current->getRightChild()->getEmployeeData();
		
		// Determine the winner
		EmployeeData* winner = nullptr;

		// If both children have data, compare their incomes
		if (leftData && rightData) {
			if (leftData->getIncome() >= rightData->getIncome()) {
				winner = leftData;
			}
			else {
				winner = rightData;
			}
		}
		// If only left child has data, it wins by default
		else if (leftData) {
			winner = leftData;
		}
		// If only right child has data, it wins by default
		else if (rightData) {
			winner = rightData;
		}

		// Set the current node's data to the winner
		current->setEmployeeData(winner);
		// Move up to the next level
		current = current->getParent();
	}
}