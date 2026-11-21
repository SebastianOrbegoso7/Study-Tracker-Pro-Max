#pragma once
#include "StudyItem.h"
#include <string>

//derived class 2
class ExamPrep : public StudyItem {
private:
	int daysUntilExam; // exam prep urgency alg. will start here
public:
	ExamPrep(string n, int c, int days) : StudyItem(n, c), daysUntilExam(days) {
		validateRange<int>(daysUntilExam, 1, 365, "Days until exam"); // validates days bounds in backend using int template
	}
	// Overriding the math AGAIN, but this time executing the urgency logic for upcoming tests.
	void calculatestudyhours(double& totalHours) const override;
	string toCSV() const override; // serializes ExamPrep row for CSV persistence
	string getType() const override { return "Exam Prep"; } // display label
	int getDaysUntilExam() const { return daysUntilExam; } // getter for GUI display
};