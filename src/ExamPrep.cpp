#include "ExamPrep.h" // removed unnecessary StandardCourse.h include
#include <iostream>
using namespace std;



void ExamPrep::calculatestudyhours(double& totalHours) const {
double urgencyBonus = (daysUntilExam < 8) ? ((8 - daysUntilExam) * 1.5) : 0.0; // prevents negative hours when exam is 8+ days away
	double hours = (CreditHours * 2.0) + urgencyBonus; // base credit hours plus non-negative urgency boost
	totalHours += hours;
}

string ExamPrep::toCSV() const { // formats ExamPrep for schedule.csv
	return "EXAMPREP," + name + "," + to_string(CreditHours) + "," + to_string(daysUntilExam);
}