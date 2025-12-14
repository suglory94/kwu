#include "TitleBST.h"


// Constructor which initializes root, parent
TitleBST::TitleBST()
	: root(nullptr), parent(nullptr) {
}

//Destructor
TitleBST::~TitleBST() {
	deleteTree(root); // Delete all nodes starting from root to free memory
}

// Insert a new node
bool TitleBST::insert(const string& artist, const string& title, const string& run_time) {
	bool success = false; // Local flag setting to true if insertion succeeds
	root = insertFunc(root, artist, title, run_time, success); // Call helper function to insert and update root
	return success; // Return whether insertion succeeds
}

// Search the titleBST for a node matching title
TitleBSTNode* TitleBST::search(const string& title) {
	TitleBSTNode* node = root->search(root, title); // Search to nodelevel method starting at root
	return node; // Return the found node
}

// Print all artist, runtime of title
void TitleBST::print() {
	printFunc(root); // Call helper function to print node
}

// Delete a matching title node
bool TitleBST::delete_node(const string& title) {
	TitleBSTNode* node = search(title); // Find the node for the given title
	if (!node) // If there is no matching title
	{
		return false; // Return false
	}
	root = deleteFunc(root, title); // Call helper function to delete and update root

	return true; // Return true
}

// Delete a single song
bool TitleBST::delete_song(const string& artist, const string& title) {
	TitleBSTNode* node = search(title); // Find the node for the given title
	if (!node) // If there is no matching title
	{
		return false; // Return false
	}
	bool deleted = node->deleteArtist(artist); // Remove the artist and store whether it was removed
	if (node->getCount() == 0) // If the title node has zero songs after deletion
	{
		root = deleteFunc(root, title); // Remove the title node and update root
	}
	return deleted; // Return whether the song was deleted
}

// Check whether the titleBST is empty
bool TitleBST::empty() {
	return root == nullptr; // Return true when root is nullptr
}