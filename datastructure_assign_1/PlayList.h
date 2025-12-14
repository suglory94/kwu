#pragma once
#include "PlayListNode.h"

class PlayList { // Definition of the PlayList class
private: // Private members
	PlayListNode* head; // Pointer to the head
	int count; // Number of nodes in the playlist
	int time; // Total runtime of all songs in the playlist

public: // Public members
	PlayList(); // Constructor declaration
	~PlayList(); // Destructor declaration
	bool insert_node(const string& artist, const string& title, int runtime_sec); // Insert an artist/title/runtime into the playlist
	bool delete_node(const string& artist, const string& title); // Delete a given song
	bool empty(); // Check if the playlist is empty
	bool full(); // Check if the playlist is full
	bool exist(const string& artist, const string& title); // Check if the song is exist in the playlist
	void print(); // Print all songs in the playlist
	int getRunTime(); // Getter for runtime
	int getCount(); // Getter for count
};