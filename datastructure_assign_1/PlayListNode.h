#pragma once
//#include "PlayList.h"
#include <string>
#include <iostream>

using namespace std;

class PlayListNode { // Definition of a PlayListNode class
private: // Private members
	string artist; // Name of the artist in this playlist
	string title; // Title of the song in this playlist
	int runtime_sec; // Runtime of the song in seconds

public: // Public members
	PlayListNode* prev; // Pointer to the previous node in the playlist
	PlayListNode* next; // Pointer to the next node in the playlist

	// Constructor which initializes artist, title, runtime, and sets prev/next nullptr
	PlayListNode(const string& artist, const string& title, int runtime_sec)
		: artist(artist), title(title), runtime_sec(runtime_sec), prev(nullptr), next(nullptr) {}
	
	// Destructor
	~PlayListNode() {}

	// Insert a new node after the current node in the playlist
	void set(PlayListNode* newNode) {
		newNode->next = this->next; // New node's next points current node's next
		newNode->prev = this; // New node's previous points current node

		if (this->next) // If the current node already has a next node
		{
			this->next->prev = newNode; // link to new node
		}
		this->next = newNode; // link the current node to the new node
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
	int getRunTimeSec() const {
		return runtime_sec;
	}
};