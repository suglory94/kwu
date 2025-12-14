#include "MusicQueue.h"


// Constructor, initializing an empty queue
MusicQueue::MusicQueue() {
	head = nullptr; // Set head pointer to nullptr
	rear = nullptr; // Set rear pointer to nullptr
}

// Destructor, deleting all nodes in the queue
MusicQueue::~MusicQueue() {
	while (!empty()) // While the queue is not empty
	{
		MusicQueueNode* temp = pop(); // Remove a node from the front
		delete temp; // Free its memory
	}
}

// Checks if the queue is empty
bool MusicQueue::empty() {
	return size == 0; // Return true if size is zero
}

// Checks if a given song exists in the queue
bool MusicQueue::exist(const string& searchArtist, const string& searchTitle) {
	MusicQueueNode* current = head; // Start from the head node
	while (current) // Loop with a while statement until the end
	{
		if (current->exist(searchArtist, searchTitle)) // If we find a matching song
		{
			return true; // Rethrn true
		}
		current = current->next; // Move to the next node
	}
	return false; // We didn't find a matching node
}

// Adds a new node to the rear of the queue
void MusicQueue::push(MusicQueueNode* newNode) {
	if (exist(newNode->getArtist(), newNode->getTitle())) // If songs duplicate
	{
		delete newNode; // Delete the duplicate node
		return; // Exit without adding
	}

	if (empty()) // If the queue is empty
	{
		head = rear = newNode; // The new node becomes both head and rear
	}
	else
	{
		rear->next = newNode; // Link current rear to the new node
		newNode->prev = rear; // Set new node's previous to the current rear
		rear = newNode; // Update rear to the new node
	}
	size++; // Increase the queue's size
}

// Removes and returns the front node of the queue
MusicQueueNode* MusicQueue::pop() {
	if (empty()) // If the queue is empty
	{
		return nullptr; // Return nullptr
	}

	MusicQueueNode* node = head; // Set node to the current head
	head = head->next; // Update head to the next node

	if (head) // If head exists
	{
		head->prev = nullptr; // Reset its previous pointer to nullptr
	}
	else // Else
	{
		rear = nullptr; // Reset rear to nullptr
	}

	node->next = nullptr; // Disconnect popped node
	size--; // Decrease the queue size
	return node; // Return the removed node
}

// Returns the front node of the queue without removing it
MusicQueueNode* MusicQueue::front() {
	if (empty()) // If the queue is empty
	{
		return nullptr; // Return nullptr
	}
	return head; // Return head
}