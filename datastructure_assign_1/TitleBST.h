#pragma once
#include "TitleBSTNode.h"

class TitleBST { // Definition of a TitleBST class
private: // Private members
	TitleBSTNode* root; // Pointer to the root node
	TitleBSTNode* parent; // Temporary pointer used for various operations

	// Helper function to insert a new artist/title/runtime into the titleBST
	TitleBSTNode* insertFunc(TitleBSTNode* node, const string& artist, const string& title, const string& run_time, bool& success) {
		if (!node) // If the current node is nullptr
		{
			success = true; // Mark insertion as successful
			return new TitleBSTNode(artist, title, run_time); // Create a new node and return
		}

		if (title < node->getTitle()) // If the new title is less than current node's title
		{
			node->left = insertFunc(node->left, artist, title, run_time, success); // Recurse left
		}
		else if (title > node->getTitle()) // If the new title is greater than current node's title
		{
			node->right = insertFunc(node->right, artist, title, run_time, success); // Recurse right
		}
		else // If the new title is same as current title
		{
			success = node->set(artist, run_time); // Add the song artist/runtime to existing title node
		}
		return node; // Return the current node
	}

	// Helper function to delete a title node from titleBST
	TitleBSTNode* deleteFunc(TitleBSTNode* node, const string& title) {
		if (!node) // If the current node is nullptr
		{
			return nullptr; // Return nullptr
		}

		if (title < node->getTitle()) // If the target title is less than current node's title
		{
			node->left = deleteFunc(node->left, title); // Recurse left
		}
		else if (title > node->getTitle()) // If the target title is greater than current node's title
		{
			node->right = deleteFunc(node->right, title); // Recurse right
		}
		else  // If the target title is same as current title
		{
			if (!node->left && !node->right)  // No children
			{
				delete node; // Delete  node
				return nullptr; // Return nullptr
			}
			else if (!node->left) // Only right child
			{
				TitleBSTNode* temp = node->right; // Save pointer to right child
				delete node; // Delete node
				return temp; // Return right child
			}
			else if (!node->right) // Only left child
			{
				TitleBSTNode* temp = node->left; // Save pointer to left child
				delete node; // Delete node
				return temp; // Return left child
			}
			else  // If node has two children
			{
				TitleBSTNode* parent = node; // Parent of the successor
				TitleBSTNode* successor = node->right; // Start with right child to find successor
				while (successor->left) // Find leftmost node in right subtree
				{
					parent = successor; // Update parent to be successor before descending
					successor = successor->left; // Descend left
				}

				if (parent != node) // If successor is not the immediate right child of node
					parent->left = successor->right; // Replace parent's left pointer
				else // If successor is immediate right child
					parent->right = successor->right; // Link node's right to successor's right

				successor->left = node->left; // Attach the deleted node's left tree to successor's left
				successor->right = node->right; // Attach the deleted node's right tree to successor's right

				delete node; // Delete the node
				return successor; // Return successor
			}
		}
		return node; // Return current node
	}

	// Helper function to print titleBST
	void printFunc(TitleBSTNode* node) {
		if (!node) // If the current node is nullptr
		{
			return; // Return
		}

		printFunc(node->left); // Print left tree first
		node->print(); // Print the current node
		printFunc(node->right); // Print right tree next
	}

	// Helper function to delete titleBST
	void deleteTree(TitleBSTNode* node) {
		if (!node) // If the current node is nullptr
		{
			return; // Return
		}

		deleteTree(node->left); // Delete left tree
		deleteTree(node->right); // Delete right tree

		delete node; // Delete current node after its children are removed
	}

public: // Public members
	TitleBST(); // Constructor declaration
	~TitleBST(); // Destructor declaration

	bool insert(const string& artist, const string& title, const string& run_time); // Insert an artist/title/runtime into the titleBST
	TitleBSTNode* search(const string& title); // Search for the given title node
	void print(); // Print all titles and their songs
	bool delete_node(const string& title); // Delete a given title node
	bool delete_song(const string& artist, const string& title); // Delete a given song node (artist, title)
	bool empty(); // Check if the tree is empty
};