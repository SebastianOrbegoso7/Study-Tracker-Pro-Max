#include "StandardCourse.h"
#include <iostream>
using namespace std;

void StandardCourse::calculatestudyhours(double& totalHours) const { // override the base class function, this is to ensure a proper replacement of the base class pure virtual
	double hours = (CreditHours * 2.0) * difficulty / 3.0; // credit*2 is for the recommended time to study per credit hour, 3 is just an average if we take difficulty out of 5
	totalHours += hours;
}

string StandardCourse::toCSV() const { // formats StandardCourse for schedule.csv
	return "STANDARD," + name + "," + to_string(CreditHours) + "," + to_string(difficulty);
}