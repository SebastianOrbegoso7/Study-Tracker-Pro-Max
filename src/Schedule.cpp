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
void Schedule::generateRecommendations() const {
	cout << "\n - - study recommendations - -\n";
	for (size_t i = 0; i < courseList.size(); i++) {
		courseList[i]->calculatestudyhours();
	}
}