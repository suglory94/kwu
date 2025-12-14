#pragma once
#include "SelectionTree.h"
#include "BpTree.h"
#include <fstream>
#include <string>

// Main class managing the application logic
class Manager
{
private:
	string cmd; // Stores the current command
	BpTree* bptree; // Pointer to the B+ Tree instance
	SelectionTree* stree; // Pointer to the Selection Tree instance

public:
	// Constructor
	Manager(int bpOrder)
	{
		flog.open("log.txt", ios::app); // Open log file

		bptree = new BpTree(&flog, bpOrder); // Create BpTree
		stree = new SelectionTree(&flog); // Create SelectionTree
	}
	// Destructor
	~Manager()
	{
		if (bptree) // Delete b+ tree
		{
			delete bptree;
			bptree = nullptr;
		}

		if (stree) // Delete seletion tree
		{
			delete stree;
			stree = nullptr;
		}

		if (flog.is_open()) // Close the output log file
			flog.close();
	}

	ifstream fin; // Input file stream
	ofstream flog;// Output file stream

	// Main function to read and execute commands
	void run(const char* command);
	void LOAD(); // Loads employee data into BpTree
	void ADD_BP(const string& cmd); // Adds a employee to BpTree
	void SEARCH_BP_NAME(string name); // Searches for a employee by name in BpTree
	void SEARCH_BP_RANGE(string start, string end); // Searches for employees within a name range in BpTree
	void PRINT_BP(); // Prints all employees from BpTree in sorted order
	void ADD_ST_DEPTNO(int dept_no); // Adds employees by department number to Selection Tree
	void ADD_ST_NAME(string name); // Adds a single employee by name to Selection Tree
	void PRINT_ST(int dept_no); // Prints employees for a specific department from Selection Tree
	void DELETE(); // Deletes the overall highest income employee from Selection Tree
	void EXIT(); // Logs exit

	void printErrorCode(int n); // Prints an error code
	void printSuccessCode(string success); // Prints a success message
};
