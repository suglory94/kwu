#pragma once
//#include "MusicQueue.h"
#include <string>
#include <cstring>
#include <sstream>

using namespace std;

class MusicQueueNode { // Defines a class representing a node in double linked list for a music queue
private:
	string artist; // Stores the artist's name
	string title; // Stores the title of song
	string run_time; // Stores the run time of song

public:
	MusicQueueNode* next; // Pointer to the next node in the list
	MusicQueueNode* prev; // Pointer to the previous node in the list

	// Constructor, initializing artist, title, and run time, and setting next, previous pointers to nullptr
	MusicQueueNode(const string& artist, const string& title, const string& run_time)
		: artist(artist), title(title), run_time(run_time), next(nullptr), prev(nullptr) {};
	
	// Destructor
	~MusicQueueNode() {}

	// Inserts a new node next to the current node
	void insert(MusicQueueNode* newNode) {
		newNode->next = this->next; // New node's next points current node's next
		newNode->prev = this; // New node's previous points current node

		if (this->next) // If the current node already has a next node
		{
			this->next->prev = newNode; // link to new node
		}
		this->next = newNode; // link the current node to the new node
	}

	// Checks if a node given exists in the linked list
	bool exist(const string& searchArtist, const string& searchTitle) {
		MusicQueueNode* current = this; // Start searching from the current node

		while (current) // Loop with a while statement until the end
		{
			// Find a matching code
			if (current->artist == searchArtist && current->title == searchTitle)
			{
				return true; // When we find a matching code, return true
			}
			current = current->next; // When we don't find a matching code, move to next node
		}

		return false; // No matching node found
	}

	// Getter for artist
	string getArtist() const {
		return artist;
	}
	
	// Getter for title
	string getTitle() const {
		return title;
	}

	// Getter for run time
	string getRunTime() const {
		return run_time;
	}
};