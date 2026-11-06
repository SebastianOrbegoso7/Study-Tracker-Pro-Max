#include "StandardCourse.h" 
#include <iostream>
#include "ExamPrep.h" 
using namespace std;

void ExamPrep::calculatestudyhours(double& totalHours) const {

    double hours = (CreditHours * 2.0) + ((8 - daysUntilExam) * 1.5);

    cout << name << " (URGENT): " << hours << " hours/week\n";

    totalHours += hours;
}