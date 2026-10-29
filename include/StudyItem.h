#pragma once
#include <string>
using namespace std;

//base class blueprint
class StudyItem {

protected: // derived classes CAN access
	string name;
	int CreditHours;

public:
	//constructor
	StudyItem(string n, int c) : name(n), CreditHours(c) {}
	virtual ~StudyItem() {} // A virtual destructor ensures that when a derived class is deleted from memory, itll clean up without junk data
	virtual void calculatestudyhours() const = 0; // =0 makes it a pure virtual class. Thisll force any class that inherits StudyItem to write its own version of this exact function.


};