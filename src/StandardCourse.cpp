#include "StandardCourse.h" 
#include <iostream>
using namespace std;

void StandardCourse::calculatestudyhours() const { // override the base class function, this is to ensure a proper replacement of the base class pure virtual
	double hours = (CreditHours * 2.0) * difficulty / 3.0; //credit*2 is for the recommended time to study per credit hour, 3 is just an average is we take difficulty out of 5
	cout << name << " (standard): " << hours << " hours of study time recommended per week." << endl;
}
