#include "Manager.h"

using namespace std; // Allows direct use of standard library names without std::

int main() // Entry point of the program
{
	Manager manager; // Create a Manager object to handle music system
	manager.run("command.txt"); // Run the manager with "command.txt"

	return 0; // Return 0 (Exit program)
}