#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

// Class to hold employee data
class EmployeeData
{
private:
    string name; // Employee's name
    int dept_no; // Department number
    int id; // Employee ID
    int income; // Employee's income

public:
    // Constructor
    EmployeeData(string name, int dept_no, int id, int income) : name(name), dept_no(dept_no), id(id), income(income) {};
    // Destructor
    ~EmployeeData() {};

    // Set all data members at once
    void setData(string name, int dept_no, int id, int income)
    {
        this->name = name;
        this->dept_no = dept_no;
        this->id = id;
        this->income = income;
    }

    // Setters
    void setName(string name) { this->name = name; }
    void setDeptNo(int dept_no) { this->dept_no = dept_no; }
    void setID(int id) { this->id = id; }
    void setIncome(int income) { this->income = income; }

    // Getters
    string getName() { return name; }
    int getDeptNo() { return dept_no; }
    int getID() { return id; }
    int getIncome() { return income; }

    // Overloaded comparison operators
    bool operator<(const EmployeeData& other) const
    {
        return this->name < other.name;
    }
    bool operator>(const EmployeeData& other) const
    {
        return this->name > other.name;
    }
    bool operator==(const EmployeeData& other) const
    {
        return this->name == other.name;
    }
    bool operator<=(const EmployeeData& other) const
    {
        return this->name <= other.name;
    }
    bool operator>=(const EmployeeData& other) const
    {
        return this->name >= other.name;
    }

    // Print employee data to console
    void print(ofstream& fout) const
    {
        fout << name << "/" << dept_no << "/" << id << "/" << income << endl;
    }
};