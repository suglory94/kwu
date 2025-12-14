#include "Manager.h"
#include <algorithm>

// Main function to run command
void Manager::run(const char* command) {
    if (flog.is_open()) {
        flog.close();
    }
    flog.open("log.txt", ios::out | ios::trunc); // Clear the log file at the start

    ifstream file(command); // Open the command file
    if (file.is_open()) {
        while (getline(file, cmd)) // Read command line
        {
            if (cmd.find("LOAD", 0) == 0) // LOAD
            {
                LOAD();
            }
            else if (cmd.find("ADD_BP", 0) == 0) // ADD_BP
            {
                ADD_BP(cmd.substr(7));
            }
            else if (cmd.find("SEARCH_BP", 0) == 0) // SEARCH_BP
            {
                string line = cmd.substr(10);
                if (line.empty()) {
                    printErrorCode(300);
                    continue;
                }
                size_t tab = line.find('\t');
                if (tab == string::npos) // Single argument
                {
                    string name = line; // Name search
                    SEARCH_BP_NAME(name);
                }
                else // Two arguments
                {
                    string start = line.substr(0, tab);
                    string end = line.substr(tab + 1);
                    if (start.empty() || end.empty()) {
                        printErrorCode(300);
                        continue;
                    }
                    SEARCH_BP_RANGE(start, end); // Range search
                }
            }
            else if (cmd.find("PRINT_BP", 0) == 0) // PRINT_BP
            {
                PRINT_BP();
            }
            else if (cmd.find("ADD_ST", 0) == 0) // ADD_ST
            {
                string line = cmd.substr(7);
                if (line.empty()) {
                    printErrorCode(500);
                    continue;
                }
                size_t tab = line.find("\t"); // Find tab
                if (line.find("dept_no") != string::npos) // Search by dept_no
                {
                    int dept_no = stoi(line.substr(tab + 1));
                    ADD_ST_DEPTNO(dept_no);
                }
                else if (line.find("name") != string::npos) // Search by name
                {
                    string name = line.substr(tab + 1);
                    ADD_ST_NAME(name);
                }
                else {
                    printErrorCode(500);
                }
            }
            else if (cmd.find("PRINT_ST", 0) == 0) // PRINT_ST
            {
                string line = cmd.substr(9);
                if (line.empty()) {
                    printErrorCode(600);
                    continue;
                }
                int dept_no = stoi(line);
                PRINT_ST(dept_no);
            }
            else if (cmd == "DELETE") // DELETE
            {
                DELETE();
            }
            else if (cmd == "EXIT") // EXIT
            {
                EXIT();
            }
            else {
                printErrorCode(800);
            }
        }
        file.close();
    }
    flog.close();
}

// Load employee data
void Manager::LOAD() {
    // Open the employee data file
    ifstream employeeFile("employee.txt");
    if (!employeeFile.is_open()) {
        printErrorCode(100);
        return;
    }

    // Check if tree is already loaded
    if (!bptree->isEmpty()) {
        printErrorCode(100);
        return;
    }

    string line;
    // Read each line from the file
    while (getline(employeeFile, line)) {
        string employeeInfo[4];
        string name;
        int dept_no, id, income;
        string delimiter = "\t"; // Parse the tab separated values
        int cur_pos = 0;
        int pos;
        int i = 0;
        while ((pos = line.find(delimiter, cur_pos)) != string::npos) {
            int len = pos - cur_pos;
            employeeInfo[i++] = line.substr(cur_pos, len);
            cur_pos = pos + 1;
        }
        employeeInfo[i] = line.substr(cur_pos);

        // Extract employee information
        name = employeeInfo[0];
        dept_no = stoi(employeeInfo[1]);
        id = stoi(employeeInfo[2]);
        income = stoi(employeeInfo[3]);

        // Create new employee data object
        EmployeeData* employee = new EmployeeData(name, dept_no, id, income);
        // Insert into B+ tree
        bptree->Insert(employee);
    }
    printSuccessCode("LOAD");
    employeeFile.close();
}

// Add a new employee to the B+ tree
void Manager::ADD_BP(const string& cmd) {
    string employeeInfo[4];
    string name;
    int dept_no, id, income;
    string delimiter = "\t"; // Parse the tab-separated values
    int cur_pos = 0;
    int pos = 0;
    int i = 0;
    while ((pos = cmd.find(delimiter, cur_pos)) != string::npos) {
        int len = pos - cur_pos;
        employeeInfo[i++] = cmd.substr(cur_pos, len);
        cur_pos = pos + 1;
    }
    employeeInfo[i++] = cmd.substr(cur_pos);

    // Validate that all fields are present
    if (employeeInfo[0].empty() || employeeInfo[1].empty() || employeeInfo[2].empty() || employeeInfo[3].empty()) {
        printErrorCode(200);
        return;
    }

    // Extract employee information
    name = employeeInfo[0];
    dept_no = stoi(employeeInfo[1]);
    id = stoi(employeeInfo[2]);
    income = stoi(employeeInfo[3]);

    // Create new employee data object
    EmployeeData* employee = new EmployeeData(name, dept_no, id, income);
    // Insert into B+ tree
    if (bptree->Insert(employee)) {
        flog << "========ADD_BP========" << endl;
        employee->print(flog);
        flog << "======================" << endl;
    }
    else {
        printErrorCode(200);
        delete employee;
    }
}

// Search for an employee by name in the B+ tree
void Manager::SEARCH_BP_NAME(string name) {
    // Check if tree is empty
    if (bptree->isEmpty()) {
        printErrorCode(300);
        return;
    }

    // Search for the data node containing the name
    BpTreeDataNode* dataNode = dynamic_cast<BpTreeDataNode*>(bptree->searchDataNode(name));
    if (!dataNode) {
        printErrorCode(300);
        return;
    }

    // Check if the name exists in the data map
    map<string, EmployeeData*>* dataMap = dataNode->getDataMap();
    if (dataMap->find(name) == dataMap->end()) {
        printErrorCode(300);
        return;
    }

    // Get and print the employee data
    EmployeeData* employee = dataMap->at(name);
    flog << "========SEARCH_BP========" << endl;
    employee->print(flog);
    flog << "=========================" << endl << endl;
}

// Search for employees within a name range in the B+ tree
void Manager::SEARCH_BP_RANGE(string start, string end) {
    // Check if tree is empty
    if (bptree->isEmpty()) {
        printErrorCode(300);
        return;
    }

    // Check if range exists
    if (!bptree->searchRange(start, end)) {
        printErrorCode(300);
        return;
    }

    flog << "========SEARCH_BP========" << endl;
    // Start from the node containing the start key
    BpTreeDataNode* dataNode = dynamic_cast<BpTreeDataNode*>(bptree->searchDataNode(start));

    // Traverse through data nodes using linked list
    while (dataNode != nullptr) {
        map<string, EmployeeData*>* dataMap = dataNode->getDataMap();

        // Check each employee in the current node
        for (auto i = dataMap->begin(); i != dataMap->end(); i++) {
            const string& key = i->first;
            // Skip keys before start of range
            if (key < start) {
                continue;
            }
            // Stop if we've gone past the end of range
            if ((key[0] != end[0]) && (key > end)) {
                break;
            }
            // Print employee within range
            EmployeeData* employee = i->second;
            employee->print(flog);
        }
        // Move to next data node
        dataNode = dynamic_cast<BpTreeDataNode*>(dataNode->getNext());
    }
    flog << "==========================" << endl;
}

// Add all employees from a specific department to the selection tree
void Manager::ADD_ST_DEPTNO(int dept_no) {
    // Check if B+ tree is empty
    if (bptree->isEmpty()) {
        printErrorCode(500);
        return;
    }

    // Navigate to the leftmost data node
    BpTreeNode* current = bptree->getRoot();
    while (dynamic_cast<BpTreeIndexNode*>(current) != nullptr) {
        BpTreeIndexNode* indexNode = dynamic_cast<BpTreeIndexNode*>(current);
        BpTreeNode* nextNode = indexNode->getMostLeftChild();
        if (nextNode == nullptr) {
            printErrorCode(500);
            return;
        }
        current = nextNode;
    }

    // Start from the node containing the start key
    BpTreeDataNode* dataNode = dynamic_cast<BpTreeDataNode*>(current);
    if (dataNode == nullptr) {
        printErrorCode(500);
        return;
    }

    bool found = false;
    // Traverse all data nodes
    while (dataNode != nullptr) {
        map<string, EmployeeData*>* dataMap = dataNode->getDataMap();
        // Check each employee
        for (auto i = dataMap->begin(); i != dataMap->end(); i++) {
            EmployeeData* employee = i->second;
            // If employee belongs to, add to selection tree
            if (employee->getDeptNo() == dept_no) {
                found = true;
                stree->Insert(employee);
            }
        }
        dataNode = dynamic_cast<BpTreeDataNode*>(dataNode->getNext());
    }

    // Print result
    if (!found) {
        printErrorCode(500);
    }
    else {
        printSuccessCode("ADD_ST");
    }
}

// Add a specific employee by name to the selection tree
void Manager::ADD_ST_NAME(string name) {
    // Check if B+ tree is empty
    if (bptree->isEmpty()) {
        printErrorCode(500);
        return;
    }

    // Search for the employee by name
    BpTreeDataNode* dataNode = dynamic_cast<BpTreeDataNode*>(bptree->searchDataNode(name));
    if (!dataNode) {
        printErrorCode(500);
        return;
    }

    // Check if the name exists
    map<string, EmployeeData*>* dataMap = dataNode->getDataMap();
    auto i = dataMap->find(name);
    if (i == dataMap->end()) {
        printErrorCode(500);
        return;
    }

    // Insert the employee into selection tree
    EmployeeData* employee = i->second;
    if (stree->Insert(employee)) {
        printSuccessCode("ADD_ST");
    }
    else {
        printErrorCode(500);
    }
}

// Print all employees in the B+ tree in sorted order
void Manager::PRINT_BP() {
    // Check if tree is empty
    if (bptree->isEmpty()) {
        printErrorCode(400);
        return;
    }

    // Navigate to the leftmost data node
    BpTreeNode* node = bptree->getRoot();
    while (node && dynamic_cast<BpTreeIndexNode*>(node)) {
        node = dynamic_cast<BpTreeIndexNode*>(node)->getMostLeftChild();
    }

    // Start from the node containing the start key
    BpTreeDataNode* dataNode = dynamic_cast<BpTreeDataNode*>(node);
    if (!dataNode) {
        printErrorCode(400);
        return;
    }

    flog << "========PRINT_BP========" << endl;
    // Traverse all data nodes using linked list
    while (dataNode != nullptr) {
        map<string, EmployeeData*>* dataMap = dataNode->getDataMap();
        // Print each employee in the current node
        for (auto i = dataMap->begin(); i != dataMap->end(); i++) {
            EmployeeData* employee = i->second;
            employee->print(flog);
        }
        dataNode = dynamic_cast<BpTreeDataNode*>(dataNode->getNext());
    }
    flog << "=========================" << endl;
}

// Print all employees in a specific department from the selection tree
void Manager::PRINT_ST(int dept_no) {
    // Validate department number
    if (dept_no % 100 != 0 || dept_no < 100 || dept_no > 800) {
        printErrorCode(600);
        return;
    }

    // Calculate the index for the department
    int index = (dept_no / 100) - 1;

    // Check if selection tree exists
    if (!stree || !stree->getRoot()) {
        printErrorCode(600);
        return;
    }

    // Get the leaf node for this department
    SelectionTreeNode* leaf = stree->getRunNode(index);
    EmployeeHeap* heap = leaf->getHeap();

    // Check if heap exists and has data
    if (!heap || heap->IsEmpty()) {
        printErrorCode(600);
        return;
    }

    // Get all employees from the heap
    int count = heap->getDataNum();
    EmployeeData** array = heap->getHeapArr();
    vector<EmployeeData*> employees;
    for (int i = 1; i <= count; i++) {
        employees.push_back(array[i]);
    }

    // Sort by income
    sort(employees.begin(), employees.end(), [](EmployeeData* a, EmployeeData* b) {
        if (a->getIncome() != b->getIncome()) {
            return a->getIncome() > b->getIncome();
        }
        return a->getName() < b->getName();
        });

    // Print all employees
    flog << "========PRINT_ST========" << endl;
    for (EmployeeData* employee : employees) {
        employee->print(flog);
    }
    flog << "=========================" << endl;
}

// Delete the employee with highest income from the selection tree
void Manager::DELETE() {
    // Check if selection tree exists
    if (!stree || stree->getRoot() == NULL) {
        printErrorCode(700);
        return;
    }

    // Attempt to delete the top employee
    if (stree->Delete()) {
        printSuccessCode("DELETE");
    }
    else {
        printErrorCode(700);
    }
}

// Exit the program and cleanup resources
void Manager::EXIT() {
    printSuccessCode("EXIT");
    // Delete B+ tree
    if (bptree != NULL) {
        delete bptree;
        bptree = NULL;
    }
    // Delete selection tree
    if (stree != NULL) {
        delete stree;
        stree = NULL;
    }
}

// Print error code
void Manager::printErrorCode(int n) {
    flog << "========ERROR========\n";
    flog << n << "\n";
    flog << "=====================\n\n";
}

// Print success message
void Manager::printSuccessCode(string success) {
    flog << "========" << success << "========\n";
    flog << "Success" << "\n";
    flog << "====================\n\n";
}