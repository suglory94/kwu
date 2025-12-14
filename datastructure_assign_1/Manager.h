#pragma once
#include "MusicQueue.h"
#include "TitleBST.h"
#include "ArtistBST.h"
#include "PlayList.h"
#include <fstream>

using namespace std; // Allows direct use of standard library names without std::

class Manager { // Definition of the Manager class
private: // Private members
	MusicQueue q; // Queue to manage songs in order
	ArtistBST ab; // Binary Search Tree storing songs by artist
	TitleBST tb; // Binary Search Tree storing songs by title
	PlayList pl; // Playlist to manage playlists
	ifstream fcmd; // Input file stream for reading command files
	ofstream flog; // Output file stream for writing logs

	// Helper function to convert a run_time string into totla seconds
	int convertToSeconds(const string& run_time) {
		size_t colon = run_time.find(':'); // Find the position of ":" in the run_time string
		if (colon == string::npos) // If ":" is not found
		{
			return 0; // Return 0
		}

		int minute = stoi(run_time.substr(0, colon)); // Extracts the substring before ":" and converts it to integer
		int second = stoi(run_time.substr(colon + 1)); // Extracts the substring after ":" and converts it to integer

		return minute * 60 + second; // Return total seconds
	}
public: // Public members
	Manager(); // Constructor declaration
	~Manager(); // Destructor declaration

	void run(const char* command); // Run the program using given command file

	void LOAD(); // Load music data into memory
	void ADD(const string& line); // Add a song using information from command line
	void QPOP(); // Remove first song from the queue
	void SEARCH(const string& line); // Search for a song based on input line
	void MAKEPL(const string& line); // Create a playlist based on input line
	void PRINT(const string& line); // Print out data
	void DELETE(const string& line); // Delete a song or playlist based on input line
	void EXIT(); // Exit the program, saving logs

};