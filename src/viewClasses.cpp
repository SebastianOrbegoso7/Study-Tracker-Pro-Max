#include <iostream>
#include <vector>
#include <string>
#include "viewClasses.h" // 1. Include the header file at the top
using namespace std;

void viewClass::viewEnteredClasses(const vector<StudyItem*>& classes) const {
	if (classes.empty()) {
		cout << "No classes entered yet." << endl;
		return;
	}
    cout << "Current Classes in Schedule:" << endl;
    for (size_t i = 0; i < classes.size(); i++) {
		cout << "- " << classes[i]->getName() << " | Credits: " << classes[i]->getCredits() << " | Study Hours: "; // Use the getter to access the name and credits
		// Call the polymorphic function to calculate study hours
		// This will automatically call the correct version of calculatestudyhours() based on the actual object type (StandardCourse or ExamPrep)
		//careful when editing future logic here, this is a polymorphic function call, meaning that the program will automatically figure out which version of the function to run based on the actual object type (StandardCourse or ExamPrep) even though the vector holds generic StudyItem pointers.
        classes[i]->calculatestudyhours();
    }

    cout << endl;
    cout << "-------------------------------" << endl;
}