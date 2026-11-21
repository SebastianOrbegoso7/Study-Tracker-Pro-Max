#pragma once
#include <string>
#include <ostream> // - needed for overloaded operator<<
#include "Validation.h" // - enforces course code and credit-hour validation in backend constructor
using namespace std;

//base class blueprint
class StudyItem {

protected: // derived classes CAN access
	string name;
	int CreditHours;

public:
	//constructor
	StudyItem(string n, int c) : name(n), CreditHours(c) {
		validateCourseCode(name); // - validates "XXX 1234" format inside backend model
		validateRange<int>(CreditHours, 1, 4, "Credit hours"); // - template call with int
	}
	virtual ~StudyItem() {} // A virtual destructor ensures that when a derived class is deleted from memory, itll clean up without junk data
	virtual void calculatestudyhours(double& totalHours) const = 0; // =0 makes it a pure virtual class. Thisll force any class that inherits StudyItem to write its own version of this exact function.
	virtual string toCSV() const = 0; // - polymorphic CSV serialization for persistence
	virtual string getType() const = 0; // - identifies derived type cleanly for GUI display

	int getCredits() const {
		return CreditHours;
	}
	string getName() const {
		return name;
	}

	friend ostream& operator<<(ostream& os, const StudyItem& item) { // - overloaded operator<< used by FileHandler::saveSchedule
		os << item.toCSV();
		return os;
	}
};