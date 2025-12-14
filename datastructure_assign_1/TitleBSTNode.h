#pragma once
#include "TitleBST.h"
#include "MusicQueueNode.h"
#include <vector>
#include <iostream>

class TitleBSTNode { // Definition of a TitleBSTNode class
private: // Private members
	string title; // Title of the song in the titleBST
	vector<string> artists; // Vector storing the artist names of the song
	vector<string> run_times; // Vector storing the run times of the song
	int count; // Count of the songs this title node holds

public: // Public members
	TitleBSTNode* left; // Pointer to the left node in the titleBST
	TitleBSTNode* right; // Pointer to the right node in the titleBST

	// Constructor which initializes title, count, and sets left/right nullptr
	TitleBSTNode(const string& artist, const string& title1, const string& run_time)
		: title(title1), count(1), left(nullptr), right(nullptr) {
		artists.push_back(artist); // Add the new artist to the artists vector
		run_times.push_back(run_time); // Add the new run time to the run_times vector
	}

	// Destructor
	~TitleBSTNode() {}

	// Insert a new artist
	bool set(const string& artist, const string& run_time) {
		if (searchArtist(artist)) // If there is already exist artist
		{
			return false; // Return false
		}

		// If there is no exist artist
		artists.push_back(artist); // Add the new artist to the artists vector
		run_times.push_back(run_time); // Add the new run time to the run_times vector
		count++; // Increase the count of artists of the title
		return true; // Return true
	}

	// Print a element matching the given artist
	void printElement(const string& artist) {
		for (size_t i = 0; i < artists.size(); i++) // Interate over all artists stored in this title node
		{
			if (artist == artists[i]) // If the current artist matches the given artist
			{
				cout << artists[i] << "/" << title << "/" << run_times[i] << endl; // Print artist/title/run time to console
			}
		}
	}

	// Print all artist, runtime of this title
	void print() {
		for (size_t i = 0; i < artists.size(); i++) // Iterate over all stored artists
		{
			cout << artists[i] << "/" << title << "/" << run_times[i] << endl; // Print artist/title/runtime to console
		}
	}

	// Search the titleBST for a node matching title
	TitleBSTNode* search(TitleBSTNode* node, const string& titleSearch) {
		if (!node) // If the current node is nullptr
		{
			return nullptr; // Return nullptr
		}

		if (titleSearch == node->getTitle()) // If the current node's title matches the searching title
		{
			return node; // Return the current node
		}

		if (titleSearch < node->getTitle()) // If the current node's title is greater than the searching title
		{
			return search(node->left, titleSearch); // Search the left tree
		}
		else // If the current node's title is less than the searching title
		{
			return search(node->right, titleSearch); // Search the right tree
		}
	}

	// Check if a given artist exists in this node
	bool searchArtist(const string& artistSearch) {
		for (size_t i = 0; i < artists.size(); i++) // Iterate through artists vector
		{
			if (artists[i] == artistSearch) // If a artist matches the searching artist
			{
				return true; // Return true
			}
		}
		return false; // Return false
	}

	// Delete a artist from this title node
	bool deleteArtist(const string& artistDelete) {
		for (size_t i = 0; i < artists.size(); i++) // Iterate through artists vector
		{
			if (artists[i] == artistDelete) // If a artist matches the deleting artist
			{
				artists.erase(artists.begin() + i); // Remove the artist from the artists vector
				run_times.erase(run_times.begin() + i); // Remove the runtime from the run_times vector
				count--; // Decrease the artist count
				return true; // Return true to indicate successful deletion
			}
		}
		return false; // Return false if the artist was not found
	}
	
	// Getter for title
	string getTitle() {
		return title;
	}

	// Setter for title
	void setTitle(const string& newTitle) {
		title = newTitle;
	}

	// Getter for artist
	string getArtist(int index) {
		return artists[index];
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