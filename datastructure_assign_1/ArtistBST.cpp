#include "ArtistBST.h"

// Constructor which initializes root, parent
ArtistBST::ArtistBST()
	: root(nullptr), parent(nullptr) {
}

// Destructor
ArtistBST::~ArtistBST() {
	deleteTree(root); // Delete all nodes starting from root to free memory
}

// Insert a new node 
bool ArtistBST::insert(const string& artist, const string& title, const string& run_time) {
	bool success = false; // Local flag setting to true if insertion succeeds
	root = insertFunc(root, artist, title, run_time, success); // Call helper function to insert and update root
	return success; // Return whether insertion succeeds
}

// Search the artistBST for a node matching artist
ArtistBSTNode* ArtistBST::search(const string& artist) {
	ArtistBSTNode* node = root->search(root, artist); // Search to nodelevel method starting at root
	return node; //Return the found node
}

// Print all title, runtime of artist
void ArtistBST::print() {
	printFunc(root); // Call helper function to print node
}

// Delete a matching artist node
bool ArtistBST::delete_node(const string& artist) {
	ArtistBSTNode* node = search(artist); // Find the node for the given artist
	if (!node) // If there is no matching artist
	{
		return false; // Return false
	}
	root = deleteFunc(root, artist); // Call helper function to delete and update root

	return true; // Return true
}

// Delete a single song
bool ArtistBST::delete_song(const string& artist, const string& title) {
	ArtistBSTNode* node = search(artist); // Find the node for the given artist
	if (!node) // If there is no matching artist
	{
		return false; // Return false
	}
	bool deleted = node->deleteTitle(title); // Remove the title and store whether it was removed
	if (node->getCount() == 0) // If the artist has zero songs after deletion
	{
		root = deleteFunc(root, artist); // Remove the artist node and update root
	}
	return deleted; // Return whether the song was deleted
}

// Check whether the artistBST is empty
bool ArtistBST::empty() {
	return root == nullptr; // Return true when root is nullptr
}