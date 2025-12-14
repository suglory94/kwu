#include "Manager.h"
using namespace std;

// Main
int main() {
	Manager manager(3); // Value is order of bptree 
	manager.run("command.txt"); // Excute commands
	return 0;
}