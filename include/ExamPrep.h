#pragma once
#include "StudyItem.h"
#include <string>

//derived class 2
class ExamPrep : public StudyItem {  //look into making this into standardcourse class
private:
	int daysUntilExam; // exam prep urgency alg. will start here
public:
	ExamPrep(string n, int c, int days) : StudyItem(n, c), daysUntilExam(days) {} // constructor again
	// Overriding the math AGAIN, but this time executing the urgency logic for upcoming tests.
	void calculatestudyhours() const override;

};