#include "PlayList.h"

// Constructor which initializes head, count, time
PlayList::PlayList() 
	: head(nullptr), count(0), time(0) {}

// Destructor
PlayList::~PlayList() {
	if (!head) // If playlist is empty
	{
		return; // Return
	}
	
	PlayListNode* current = head; // Start from the head node
	do
	{
		PlayListNode* next = current->next; // Store next node
		delete current; // Delete the current node
		current = next; // Move to the next node
	} while (current != head); // Stop when current == head (circular list)
}

// Insert a new node
bool PlayList::insert_node(const string& artist, const string& title, int runtime_sec) {
	if (full() || (head && exist(artist, title))) // If the playlist is full or the song exists
	{
		return false; // Return false
	}

	PlayListNode* newNode = new PlayListNode(artist, title, runtime_sec); // Create a new song node

	if (!head) // If playlist is empty
	{
		head = newNode; // New node becomes head
		head->next = head; // Next pointer points itself
		head->prev = head; // Prev pointer points itself
	}
	else // If playlist has songs
	{
		PlayListNode* tail = head->prev; // Get the last node
		tail->next = newNode; // Link last node to new node
		newNode->prev = tail; // New node's prev points to tail
		newNode->next = head; // New node's next points to head
		head->prev = newNode; // Head's prev points to new node
	}

	count++; // Increase count
	time += runtime_sec; // Increase total runtime
	return true; // Return true
}

// Delete a matching song node
bool PlayList::delete_node(const string& artist, const string& title) {
	if (!head) // If playlist is emtpy
	{
		return false; // Return false
	}

	PlayListNode* current = head; // Start from head

	do {
		if (current->getArtist() == artist && current->getTitle() == title) // If we find the song that matches artist and title
		{
			count--; // Decrease count
			time -= current->getRunTimeSec(); // Decrease total runtime

			if (current == head && current->next == head) // If playlist has one song
			{
				delete current; // Delete the node
				head = nullptr; // Playlist becomes empty
				return true; // Return true
			}

			if (current == head) // If we delete the head but more than one node exists
			{
				head = head->next; // Move head to the next node
			}

			current->prev->next = current->next; // Update links of surrounding nodes to delete current
			current->next->prev = current->prev;

			delete current; // Delete the current node
			return true; // Return true
		}

		current = current->next; // Move to the next node
	} while (current != head); // Stop when current == head(circular list)

	return false; // Return false if there is no song found
}

// Check whether the playlist is empty
bool PlayList::empty() {
	return count == 0; // Return true if the count == 0
}

// Check whether the playlist is full
bool PlayList::full() {
	return count == 10; // Return true if the count == 10
}

// Check whether the song is exist in the playlist
bool PlayList::exist(const string& artistSearch, const string& titleSearch) {
	if (!head) // If playlist is empty
	{
		return false; // Return false
	}
	
	PlayListNode* current = head; // Start from head
	do
	{
		if ((current->getArtist() == artistSearch) && (current->getTitle() == titleSearch)) // If the matching song found
		{
			return true; // Return true
		}
		current = current->next; // Move to the next node
	} while (current != head); // Stop when current == head (circular list)

	return false; // Return false if there is no song found
}

// Print all songs in the playlist
void PlayList::print() {
	if (empty()) // If the play list is full
	{
		return; // Return
	}

	PlayListNode* current = head; // Start from head
	int loop = count; // Loop playlist
	while (loop--) // Loop count times
	{
		int min = current->getRunTimeSec() / 60; // Convert runtime to minutes
		int sec = current->getRunTimeSec() % 60; // Convert remaining runtime to seconds
		cout << current->getArtist() << "/" << current->getTitle() << "/" << min << ":" << (sec < 10 ? "0" : "") << sec << endl; // Print output
		current = current->next; // Move to the next node
	}

	int minTotal = time / 60; // Total minutes
	int secTotal = time % 60; // Remaining seconds

	cout << "Count : " << count << " / 10" << endl; // Print total number of songs
	cout << "Time : " << minTotal << "min " << secTotal << "sec" << endl; // Print total runtime
}

// Getter for runtime
int PlayList::getRunTime() {
	return time;
}

// Getter for count
int PlayList::getCount() {
	return count;
}