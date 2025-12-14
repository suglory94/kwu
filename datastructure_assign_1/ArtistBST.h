#pragma once
#include "ArtistBSTNode.h"

class ArtistBST { // Definition of a ArtistBST class
private: // Private members
	ArtistBSTNode* root; // Pointer to the root node
	ArtistBSTNode* parent; // Temporary pointer used for various operations

	// Helper function to insert a new artist/title/runtime into the aritstBST
	ArtistBSTNode* insertFunc(ArtistBSTNode* node, const string& artist, const string& title, const string& run_time, bool& success) {
		if (!node) // If the current node is nullptr
		{
			success = true; // Mark insertion as successful
			return new ArtistBSTNode(artist, title, run_time); // Create a new node and return
		}
		
		if (artist < node->getArtist()) // If the new artist is less than current node's artist
		{
			node->left = insertFunc(node->left, artist, title, run_time, success); // Recurse left
		}
		else if (artist > node->getArtist()) // If the new artist is greater than current node's artist
		{
			node->right = insertFunc(node->right, artist, title, run_time, success); // Recurse rigiht
		}
		else // If the new artist is same as current artist
		{
			success = node->set(title, run_time); // Add the song title/runtime to existing artist node
		}
		return node; // Return the current node
	}

	// Helper function to delete an artist node from artistBST
	ArtistBSTNode* deleteFunc(ArtistBSTNode* node, const string& artist) {
		if (!node) // If the current node is nullptr
		{
			return nullptr; // Return nullptr
		}

		if (artist < node->getArtist()) // If the target artist is less than current node's artist
		{
			node->left = deleteFunc(node->left, artist); // Recurse left
		}
		else if (artist > node->getArtist()) // If the target artist is greater than current node's artist
		{
			node->right = deleteFunc(node->right, artist); // Recurse right
		}
		else // If the target artist is same as current artist
		{
			if (!node->left && !node->right) // No children
			{
				delete node; // Delete node
				return nullptr; // Return nullptr
			}
			else if (!node->left) // Only right child
			{
				ArtistBSTNode* temp = node->right; // Save pointer to right child
				delete node; // Delete node
				return temp; // Return right child
			}
			else if (!node->right) // Only left child
			{
				ArtistBSTNode* temp = node->left; // Save pointer to left child
				delete node; // Delete node
				return temp; // Return left child
			}
			else // If node has two children
			{
				ArtistBSTNode* parent = node; // Parent of the successor
				ArtistBSTNode* successor = node->right; // Start with right child to find successor
				while (successor->left) // Find leftmost node in right subtree
				{
					parent = successor; // Update parent to be successor before descending
					successor = successor->left; // Descend left
				}

				if (parent != node) // If successor is not the immediate right child of node
					parent->left = successor->right; // Replace parent's left pointer
				else // If successor is immediate right child
					parent->right = successor->right; // Link node's right to succesor's right

				successor->left = node->left; // Attach the deleted node's left tree to successor's left
				successor->right = node->right; // Attach the deleted node's right tree to successor's right

				delete node; // Delete the node
				return successor; // Return successor
			}
		}
		return node; // Return current node
	}

	// Helper function to print artistBST
	void printFunc(ArtistBSTNode* node) {
		if (!node) // If the current node is nullptr
		{
			return; // Return
		}

		printFunc(node->left); // Print left tree first
		node->print(); // Print the current node
		printFunc(node->right); // Print right tree next
	}

	// Helper function to delete artistBST
	void deleteTree(ArtistBSTNode* node) {
		if (!node) // If the current node is nullptr
		{
			return; // Return
		}

		deleteTree(node->left); // Delete left tree
		deleteTree(node->right); // Delete right tree

		delete node; // Delete current node after its children are removed
	}

public: // Public members
	ArtistBST(); // Constructor declaration
	~ArtistBST(); // Destructor declaration

	bool insert(const string& artist, const string& title, const string& run_time); // Insert an artist/title/runtime into the artistBST
	ArtistBSTNode* search(const string& artist); // Search for the given artist node
	void print(); // Print all artists and their songs
	bool delete_node(const string& artist); // Delete a given artist node
	bool delete_song(const string& artist, const string& title); // Delete a given song node (artist, title)
	bool empty(); // Check if the tree is empty
};