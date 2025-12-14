#include "Manager.h"
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std; // Allows direct use of standard library names without std::

// Constructor
Manager::Manager() {

}

// Destructor
Manager::~Manager() {

}

// Main execution function processing a command file
void Manager::run(const char* command) {
	string line; // Each line from the command file
	ifstream file(command); // Open the given file
	ofstream logFile("log.txt", ios::app);
	streambuf* backup = cout.rdbuf();
	cout.rdbuf(logFile.rdbuf());
	if (file.is_open()) // If file opened successfully
	{
		while (getline(file, line)) // Read file line
		{
			if (line == "LOAD") // If command is LOAD
			{
				LOAD(); // Call LOAD function
			}
			else if (line.find("ADD", 0) == 0) // If command starts with ADD
			{
				ADD(line.substr(4)); // Call ADD function passing the substring after ADD
			}
			else if (line == "QPOP") // If command is QPOP
			{
				QPOP(); // Call QPOP function
			}
			else if (line.find("SEARCH", 0) == 0) //If command starts with SEARCH
			{
				SEARCH(line.substr(7)); // Call SEARCH function passing the substring after SEARCH
			}
			else if (line.find("MAKEPL", 0) == 0) // If command starts with MAKEPL
			{
				MAKEPL(line.substr(7)); // Call MAKEPL function passing the substring after MAKEPL
			}
			else if (line.find("PRINT", 0) == 0) // If command starts with PRINT
			{
				PRINT(line.substr(6)); // Call PRINT function passing the substring after PRINT
			}
			else if (line.find("DELETE", 0) == 0) // If command starts with DELETE
			{
				DELETE(line.substr(7)); // Call DELETE function passing the substring after DELETE
			}
			else if (line == "EXIT") // If command is EXIT
			{
				EXIT(); // Call EXIT function
				break; // Exit loop
			}
			else // If command not recognized
			{
				cout << "========ERROR========" << endl;
				cout << 1000 << endl; // Print error code
				cout << "====================" << endl;
			}
		}
		file.close(); // Close the input file
	}
	else // If file could not be opened
	{
		cout << "========ERROR========" << endl;
		cout << 100 << endl; // Print error code
		cout << "====================" << endl;
	}

	cout.rdbuf(backup);
	logFile.close();
}

// Load songs from "Music_list.txt" into the queue
void Manager::LOAD() {
	if (!q.empty()) // If queue already has data
	{
		cout << "========ERROR========" << endl;
		cout << 100 << endl; // Print error code
		cout << "====================" << endl;
		return; // Return
	}

	// Open the music list file
	ifstream file("Music_List.txt");
	if (file.is_open()) // If file is opened
	{
		string line; // Helper variable to getline
		cout << "========LOAD========" << endl;
		while (getline(file, line)) // Read file line
		{
			string artist, title, run_time; // Artist, title, runtime variable
			string songInfo[3]; // Array to store split info
			string delimiter = "|"; // Delimiter between fields
			
			int cur_pos = 0; // Current parsing position
			int pos; // Position of delimiter
			int i = 0; // Index counter
			while ((pos = line.find(delimiter, cur_pos)) != string::npos) // Parse until no delimeter
			{
				int len = pos - cur_pos; // Length of token
				songInfo[i++] = line.substr(cur_pos, len); // Extract substring
				cur_pos = pos + 1; // Move position after delimiter
			}
			songInfo[i] = line.substr(cur_pos); // Store last token

			artist = songInfo[0]; // First field is artist
			title = songInfo[1]; // Second field is title
			run_time = songInfo[2]; // Third field is runtime

			q.push(new MusicQueueNode(artist, title, run_time)); // Push new node to queue

			cout << artist << "/" << title << "/" << run_time << endl; // Print song info
		}
		cout << "====================" << endl;
		file.close(); // Close the music list file
	}
}

// Add a song using information from command line
void Manager::ADD(const string& line) {
	string artist, title, run_time; // Artist, title, runtime variable
	string songInfo[3]; // Array to store split info
	string delimiter = "|"; // Delimiter between fields

	int cur_pos = 0; // Current parsing position
	int pos; // Position of delimiter
	int i = 0; // Index counter
	while ((pos = line.find(delimiter, cur_pos)) != string::npos) // Parse until no delimiter
	{
		int len = pos - cur_pos; // Length of token
		songInfo[i++] = line.substr(cur_pos, len); // Extract substring
		cur_pos = pos + 1; // Move position after delimiter
	}
	songInfo[i] = line.substr(cur_pos); // Store last token

	artist = songInfo[0]; // First field is artist
	title = songInfo[1]; // Second field is title
	run_time = songInfo[2]; // Third field is runtime

	if (artist.empty() || title.empty() || run_time.empty() || q.exist(artist, title)) // If artist, title, runtime is not validate or exist in queue
	{
		cout << "========ERROR========" << endl;
		cout << 200 << endl; // Print error
		cout << "======================" << endl;
		return; // Return
	}
	
	q.push(new MusicQueueNode(artist, title, run_time)); // Push new node to queue

	cout << "========ADD========" << endl;
	cout << artist << "/" << title << "/" << run_time << endl; // Print add song info
	cout << "====================" << endl;
}

// Remove first song from the queue
void Manager::QPOP() {
	if (q.empty()) // If queue is empty
	{
		cout << "========ERROR========" << endl;
		cout << 300; // Print error
		cout << "=====================" << endl;
		return; // Return
	}
	
	while (!q.empty()) // While queue is not empty
	{
		MusicQueueNode* node = q.pop(); // Pop node from queue
		
		string artist = node->getArtist(); // Get artist
		string title = node->getTitle(); // Get title
		string run_time = node->getRunTime(); // Get runtime

		if (!ab.insert(artist, title, run_time)) // Try inserting into artistBST
		{
			cout << "========ERROR========" << endl; // If fail
			cout << 300 << endl; // Print error
			cout << "=====================" << endl;
			delete node; // Free memory
			return; // Return
		}

		if (!tb.insert(artist, title, run_time)) // Try inserting into titleBST
		{
			cout << "========ERROR========" << endl; // If fail
			cout << 300 << endl; // Print error
			cout << "=====================" << endl;
			delete node; // Free memory
			return; // Return
		}

		delete node; // Free popped node
	}
	cout << "========QPOP========" << endl;
	cout << "Success" << endl; // Print qpop result
	cout << "===================" << endl;
}

// Search for a song based on input line
void Manager::SEARCH(const string& line) {
	string option; // Search option (ARTIST, TITLE, SONG)
	string songSearch; // Query string
	size_t spacePos = line.find(' '); // Find first space position

	option = line.substr(0, spacePos); // Extract option before space
	songSearch = line.substr(spacePos + 1); // Extract song to search after space
	
	if (option == "ARTIST") // If searching by ARTIST
	{
		ArtistBSTNode* node = ab.search(songSearch); // Search in artistBST
		if (node) // If node found
		{
			cout << "========SEARCH========" << endl;
			node->print(); // Print SEARCH result (artist info)
			cout << "====================" << endl;
		}
		else // If node not found
		{
			cout << "========ERROR========" << endl;
			cout << 400 << endl; // Print error
			cout << "=====================" << endl;
			return; // Return
		}
	}
	else if (option == "TITLE") // If searching by TITLE
	{
		TitleBSTNode* node = tb.search(songSearch); // Search in titleBST
		if (node) // If node found
		{
			cout << "========SEARCH========" << endl;
			node->print(); // Print SEARCH result (title info)
			cout << "====================" << endl;
		}
		else // If node not found
		{
			cout << "========ERROR========" << endl;
			cout << 400 << endl; // Print error
			cout << "=====================" << endl;
			return; // Return
		}
	}
	else if (option == "SONG") // If searching by SONG
	{
		string songSearch_artist; // Artist from query
		string songSearch_title; // Title from query
		size_t spacePos = songSearch.find('|'); // Find delimiter
		songSearch_artist = songSearch.substr(0, spacePos); // Extract artist
		songSearch_title = songSearch.substr(spacePos + 1); // Extract title

		ArtistBSTNode* node = ab.search(songSearch_artist); // Search artist
		if (node && node->searchTitle(songSearch_title)) // If artist and title found
		{
			cout << "========SEARCH========" << endl; 
			node->printElement(songSearch_title); // Print SEARCH result (song info)
			cout << "====================" << endl;
		}
		else // If artist and title not found
		{
			cout << "========ERROR========" << endl;
			cout << 400 << endl; // Print error
			cout << "=====================" << endl;
			return; // Return
		}
	}
	else // If option is anything else
	{
		cout << "========ERROR========" << endl;
		cout << 400 << endl; // Print error
		cout << "=====================" << endl;
		return; // Return
	}
}

// Create a playlist based on input line
void Manager::MAKEPL(const string& line) {
	string option; // Search option (ARTIST, TITLE, SONG)
	string songSearch; // Query string
	size_t spacePos = line.find(' '); // Find first space position

	option = line.substr(0, spacePos); // Extract option before space
	songSearch = line.substr(spacePos + 1); // Extract query after space

	if (option == "ARTIST") // If making playlist by artist
	{
		ArtistBSTNode* node = ab.search(songSearch); // Search in artistBST

		if (node && (node->getCount() <= 10 - pl.getCount())) // If found and enough space in playlist
		{
			for (size_t i = 0; i < node->getCount(); i++) // Iterate through all artist node
			{
				if (pl.exist(songSearch, node->getTitle(i))) // If a song exists in playlist
				{
					cout << "========ERROR========" << endl;
					cout << 500 << endl; // Print error
					cout << "=====================" << endl;
					return; // Return
				}
			}
			for (size_t i = 0; i < node->getCount(); i++) // Iterate through all artist node
			{
				string title = node->getTitle(i); // Get song title
				string run_time = node->getRunTime(i); // Get runtime
				int runtime_sec = convertToSeconds(run_time); // Convert runtime to seconds

				if (!pl.insert_node(songSearch, title, runtime_sec)) // Insert into playlist
				{
					cout << "========ERROR========" << endl; // If fail
					cout << 500 << endl; // Print error
					cout << "=====================" << endl;
					return; // Return
				}
			}
		}
		else // Else
		{
			cout << "========ERROR========" << endl;
			cout << 500 << endl; // Print error
			cout << "=====================" << endl;
			return; // Return
		}

		cout << "========MAKEPL========" << endl;
		pl.print(); // Print MAKEPL result (playlist info)
		cout << "======================" << endl;
	}
	else if (option == "TITLE") // If making playlist by title
	{
		TitleBSTNode* node = tb.search(songSearch); // Search in titleBST
		if (node && (node->getCount() <= 10 - pl.getCount())) // If found and enough space in playlist
		{
			for (size_t i = 0; i < node->getCount(); i++) // Iterate through all title node
			{
				if (pl.exist(node->getArtist(i), songSearch)) // If a song exists in playlist
				{
					cout << "========ERROR========" << endl;
					cout << 500 << endl; // Print error
					cout << "=====================" << endl;
					return; // Return
				}
			}
			for (size_t i = 0; i < node->getCount(); i++) // Iterate through all title node
			{
				string artist = node->getArtist(i); // Get song artist
				string run_time = node->getRunTime(i); // Get runtime
				int runtime_sec = convertToSeconds(run_time); // Convert runtime to seconds

				if (!pl.insert_node(artist, songSearch, runtime_sec)) // Insert into playlist
				{
					cout << "========ERROR========" << endl; // If fail
					cout << 500 << endl; // Print error
					cout << "=====================" << endl;
					return; // Return
				}
			}
		}
		else // Else
		{
			cout << "========ERROR========" << endl;
			cout << 500 << endl; // Print error
			cout << "=====================" << endl;
			return; // Return
		}
		cout << "========MAKEPL========" << endl;
		pl.print(); // Print MAKEPL result (playlist info)
		cout << "======================" << endl;
	}
	else if (option == "SONG") // If making playlist by song
	{
		string songSearch_artist; // Extract artist
		string songSearch_title; // Extract title
		size_t spacePos = songSearch.find('|'); // Find delimiter
		songSearch_artist = songSearch.substr(0, spacePos); // Artist
		songSearch_title = songSearch.substr(spacePos + 1); // Title

		ArtistBSTNode* node = ab.search(songSearch_artist); // Search in artistBST
		if (node && node->searchTitle(songSearch_title)) // If found and song exists
		{
			for (size_t i = 0; i < node->getCount(); i++) { // Iterate through artist node
				string run_time = node->getRunTime(i); // Get runtime
				int runtime_sec = convertToSeconds(run_time); // Convert runtime to seconds

				if (!pl.insert_node(songSearch_artist, songSearch_title, runtime_sec)) // Insert into playlist
				{
					cout << "========ERROR========" << endl; // If fail
					cout << 500 << endl; // Print error
					cout << "=====================" << endl;
					return; // Return
				}
			}
		}
		else // Else
		{
			cout << "========ERROR========" << endl;
			cout << 500 << endl; // Print error
			cout << "=====================" << endl;
			return; // Return
		}
		cout << "========MAKEPL========" << endl;
		pl.print(); // Print MAKEPL result (playlist info)
		cout << "======================" << endl;
	}
	else // Else
	{
		cout << "========ERROR========" << endl;
		cout << 500 << endl; // Print error
		cout << "=====================" << endl;
		return; // Return
	}
}

// Print out data
void Manager::PRINT(const string& line) {
	if (line == "ARTIST") // If print artist
	{
		if (ab.empty()) // If artistBST is empty
		{
			cout << "========ERROR========" << endl;
			cout << 600 << endl; // Print error
			cout << "=====================" << endl;
			return; // Return
		}

		cout << "========PRINT========" << endl;
		cout << "ArtistBST" << endl;
		ab.print(); // Print PRINT result (artistBST)
		cout << "=====================" << endl;
	}
	else if (line == "TITLE") // If print title
	{
		if (tb.empty()) // If titleBST is empty
		{
			cout << "========ERROR========" << endl;
			cout << 600 << endl; // Print error
			cout << "=====================" << endl;
			return; // Return
		}

		cout << "========PRINT========" << endl;
		cout << "TitleBST" << endl;
		tb.print(); // Print PRINT result (titleBST)
		cout << "=====================" << endl;
	}
	else if (line == "LIST") // If print playlist
	{
		if (pl.empty()) // If playlist is empty
		{
			cout << "========ERROR========" << endl;
			cout << 600 << endl; // Print error
			cout << "=====================" << endl;
			return; // Return
		}

		cout << "========PRINT========" << endl;
		pl.print(); // Print PRINT result (playlist)
		cout << "=====================" << endl;
	}
}

// Delete a song or playlist based on input line
void Manager::DELETE(const string& line) {
	string option; // Search option (ARTIST, TITLE, LIST, SONG)
	string songSearch; // Query string
	size_t spacePos = line.find(' '); // Find first space position

	option = line.substr(0, spacePos); // Extract option before space
	songSearch = line.substr(spacePos + 1); // Extract search song after space

	if (option == "ARTIST") // If deleting song by artist
	{
		ArtistBSTNode* node = ab.search(songSearch); // Search in artistBST
		if (!node)  // If node not found
		{ 
			cout << "========ERROR========" << endl; 
			cout << 700 << endl; // Print error
			cout << "=====================" << endl;
			return; // Return
		} 
		for (int i = 0; i < node->getCount(); i++)  // Iterate through artist node
		{ 
			string title = node->getTitle(i);  // Get title
			tb.delete_song(songSearch, title); // Delete song in titleBST
			pl.delete_node(songSearch, title); // Delete song in playlist
		} 

		if (!ab.delete_node(songSearch)) // Delete artist node
		{
			cout << "========ERROR========" << endl;
			cout << 700 << endl; // Print error
			cout << "=====================" << endl;
			return; // Return
		}

		cout << "========DELETE========" << endl; 
		cout << "Success" << endl; // Print DELETE result 
		cout << "====================" << endl;
	}
	else if (option == "TITLE") // If deleting song by title
	{
		TitleBSTNode* node = tb.search(songSearch); // Search in titleBST
		if (!node) // If node not found
		{
			cout << "========ERROR========" << endl;
			cout << 700 << endl; // Print error
			cout << "=====================" << endl;
			return; // Return
		}
		for (int i = 0; i < node->getCount(); i++) // Iterate through titleNode
		{
			string artist = node->getArtist(i); // Get artist
			ab.delete_song(artist, songSearch); // Delete song in artistBST
			pl.delete_node(artist, songSearch); // Delete song in playlist
		}
		if (!tb.delete_node(songSearch)) // Delete title node
		{
			cout << "========ERROR========" << endl;
			cout << 700 << endl; // Print error
			cout << "=====================" << endl;
			return; // Return
		}
		cout << "========DELETE========" << endl;
		cout << "Success" << endl; // Print DELETE result
		cout << "====================" << endl;
	}
	else if (option == "LIST") // If deleting song by list
	{
		string songSearch_artist; // Extract artist
		string songSearch_title; // Extract title
		size_t spacePos = songSearch.find('|'); // Find delimiter
		songSearch_artist = songSearch.substr(0, spacePos); // Artist
		songSearch_title = songSearch.substr(spacePos + 1); // Title

		if (!pl.delete_node(songSearch_artist, songSearch_title)) // Delete play list node
		{
			cout << "========ERROR========" << endl;
			cout << 700 << endl; // Print error
			cout << "=====================" << endl;
			return; // Return
		}
		
		cout << "========DELETE========" << endl;
		cout << "Success" << endl; // Print DELETE result
		cout << "====================" << endl;
	}
	else if (option == "SONG") // If deleting song
	{
		string songSearch_artist; // Extract artist
		string songSearch_title; // Extract title
		size_t spacePos = songSearch.find('|'); // Find delimiter
		songSearch_artist = songSearch.substr(0, spacePos); // Artist
		songSearch_title = songSearch.substr(spacePos + 1); // Title

		if (!ab.delete_song(songSearch_artist, songSearch_title)) // Delete artist node
		{
			cout << "========ERROR========" << endl;
			cout << 700 << endl; // Print error
			cout << "=====================" << endl;
			return; // Return
		}
		if (!tb.delete_song(songSearch_artist, songSearch_title)) // Delete title node
		{
			cout << "========ERROR========" << endl;
			cout << 700 << endl; // Print error
			cout << "=====================" << endl;
			return; // Return
		}
		pl.delete_node(songSearch_artist, songSearch_title); // Delete play list

		cout << "========DELETE========" << endl;
		cout << "Success" << endl; // Print DELETE result
		cout << "====================" << endl;
	}
	else // Else
	{
		cout << "========ERROR========" << endl;
		cout << 400 << endl; // Print error
		cout << "=====================" << endl;
		return; // Return
	}
}

// Exit the program, saving logs
void Manager::EXIT() {
	cout << "=======EXIT========" << endl;
	cout << "Success" << endl; // Print EXIT result
	cout << "===================" << endl;
}