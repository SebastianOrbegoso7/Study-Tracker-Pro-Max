#include <iostream>
#include "ClassSchedule.h"
using namespace std;

Schedule::~Schedule() {
	for (size_t i = 0; i < courseList.size(); i++) { // The destructor loops through the vector and deletes the pointers to free up RAM.
		delete courseList[i];
	}
}
void Schedule::addCourse(StudyItem* course) {
	courseList.push_back(course); // adds a new course to the vector
}

// polymorphic user task, even though the vector holds generic "StudyItem" pointers the program automatically figures out whether to run the Standard math or the ExamPrep math for each specific item.

void Schedule::generateRecommendations(double weeklyLimit) const {

    cout << "\n--- Study Recommendations ---\n";

    double totalHours = 0.0;

    for (size_t i = 0; i < courseList.size(); i++) {
        courseList[i]->calculatestudyhours(totalHours);
    }

    cout << "\nTotal recommended study hours: "
        << totalHours << endl;

    cout << "Your weekly study-hour limit: "
        << weeklyLimit << endl;

    if (totalHours > weeklyLimit) {
        cout << "\nWARNING: Your recommended study hours exceed your weekly limit!"
            << endl;
        cout << "Consider adjusting your study schedule." << endl;
    }
    else {
        cout << "\nYour study recommendations are within your weekly limit!"
            << endl;
    }
}