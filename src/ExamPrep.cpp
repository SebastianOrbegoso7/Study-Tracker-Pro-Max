#include "StandardCourse.h" 
#include <iostream>
#include "ExamPrep.h" 
using namespace std;


void ExamPrep::calculatestudyhours() const {
	double hours = (CreditHours * 2.0) + ((8 - daysUntilExam) * 1.5);// 8 is used for the urgency logic, an example is 8-1=7so test is one day away, MASSIVE urgency boost, 7 is the urgency SCORE**
	cout << name << " (URGENT): " << hours << " hours/week\n";
}