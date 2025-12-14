#pragma once
#include "ArtistBST.h"
#include "MusicQueueNode.h"
#include <vector>
#include <iostream>

class ArtistBSTNode { // Definition of a ArtistBSTNode class
private: // Private members
	string artist; // Name of the artist in the artistBST
	vector<string> titles; // Vector storing the song titles of the artist
	vector<string> run_times; // Vector storing the song run times of the artist
	int count; // Count of the songs this artist node holds

public: // Public memers
	ArtistBSTNode* left; // Pointer to the left node in the artistBST
	ArtistBSTNode* right; // Pointer to the right node in the artistBST

	// Constructor which initializes artist, count, and sets left/right nullptr
	ArtistBSTNode(const string& artist1, const string& title, const string& run_time)
		: artist(artist1), count(1), left(nullptr), right(nullptr) {
		titles.push_back(title); // Add the new title to the titles vector
		run_times.push_back(run_time); // Add the new run time to the run_times vector
	}

	// Destructor
	~ArtistBSTNode() {}

	// Insert a new title
	bool set(const string& title, const string& run_time) {
		if (searchTitle(title)) // If there is already exist title
		{
			return false; // Return false
		}

		// If there is no exist title
		titles.push_back(title); // Add the new title to the titles vector
		run_times.push_back(run_time); // Add the new run time to the run_times vector
		count++; // Increase the count of titles of the artist
		return true; // Return true
	}

	// Print a element matching the given title
	void printElement(const string& title) {
		for (size_t i = 0; i < titles.size(); i++) // Iterate over all titles stored in this artist node
		{
			if (title == titles[i]) // If the current title matches the given title
			{
				cout << artist << "/" << titles[i] << "/" << run_times[i] << endl; // Print artist/title/run time to console
			}
		}
	}

	// Print all title, runtime of this artist
	void print() {
		for (size_t i = 0; i < titles.size(); i++) // Iterate over all stored titles
		{
			cout << artist << "/" << titles[i] << "/" << run_times[i] << endl; // Print artist/title/runtime to console
		}
	}

	// Search the artistBST for a node matching artist
	ArtistBSTNode* search(ArtistBSTNode* node, const string& artistSearch) {
		if (!node) // If the current node is nullptr
		{
			return nullptr; // Return nullptr
		}
		
		if (artistSearch == node->getArtist()) // If the current node's artist matches the searching artist
		{
			return node; // Return the current node
		}

		if (artistSearch < node->getArtist()) // If the current node's artist is greater than the searching artist
		{
			return search(node->left, artistSearch); // Search the left tree
		}
		else // If the current node's artist is less than the searching artist
		{
			return search(node->right, artistSearch); // Search the right tree
		}
	}

	// Check if a given title exists in this node
	bool searchTitle(const string& titleSearch) {
		for (size_t i = 0; i < titles.size(); i++) // Iterate through titles vector
		{
			if (titles[i] == titleSearch) // If a title matches the searching title
			{
				return true; // Return true
			}
		}
		return false; // Return false
	}

	// Delete a title from this artist node
	bool deleteTitle(const string& titleDelete) {
		for (size_t i = 0; i < titles.size(); i++) // Iterate through titles vector
		{
			if (titles[i] == titleDelete) // If a title matches the deleting title
			{
				titles.erase(titles.begin() + i); // Remove the title from the titles vector
				run_times.erase(run_times.begin() + i); // Remove the runtime from the run_times vector
				count--; // Decrease the title count
				return true; // Return true to indicate successful deletion
			}
		}
		return false; // Return false if the title was not found
	}

	// Getter for artist
	string getArtist() {
		return artist;
	}

	// Setter for artist
	void setArtist(const string& newArtist) {
		artist = newArtist;
	}

	// Getter for title
	string getTitle(int index) {
		return titles[index];
	}

	// Getter for runtime
	string getRunTime(int index) {
		return run_times[index];
	}

	// Getter for count
	int getCount() {
		return count;
	}
};