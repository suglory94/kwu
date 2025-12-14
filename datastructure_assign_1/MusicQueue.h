#pragma once
#include "MusicQueueNode.h"

class MusicQueue { // Defines a class representing a music queue using a linked list
private:
	MusicQueueNode* head; // Pointer to the head node of the queue
	MusicQueueNode* rear; // Pointer to the rear node of the queue
	int size = 0; // Stores the number of nodes in queue
public:
	MusicQueue(); // Constructor declaration
	~MusicQueue(); // Destructor declaration
	
	bool empty(); // Checks if the queue is empty
	bool exist(const string& searchArtist, const string& searchTitle); // Checks if a given song exists in the queue
	void push(MusicQueueNode* newNode); // Adds a new node in the rear of the queue
	MusicQueueNode* pop(); // Removes and returns the node in the head of the queue
	MusicQueueNode* front(); // Returns a pointer to the front node without removing it
};