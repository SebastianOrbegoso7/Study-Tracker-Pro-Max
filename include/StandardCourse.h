#pragma once
#include "StudyItem.h"
#include <string>
using namespace std;

class StandardCourse : public StudyItem { //first derived class
private:
	int difficulty; // for our difficulty level track

public:
	StandardCourse(string n, int c, int diff) : StudyItem(n, c), difficulty(diff) {
		validateRange<int>(difficulty, 1, 5, "Difficulty"); // validates difficulty bounds in backend using int template
	}

	void calculatestudyhours(double& totalHours) const override;
	string toCSV() const override; // serializes StandardCourse row for CSV persistence
	string getType() const override { return "Standard"; } // display label
	int getDifficulty() const { return difficulty; } // getter for GUI display
};