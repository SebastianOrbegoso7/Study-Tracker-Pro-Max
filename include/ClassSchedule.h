#pragma once
#include <vector>
#include "StudyItem.h"
using namespace std;

class Schedule {
private:
	vector<StudyItem*> courseList; // vector of pointers to StudyItem objects
public:
	~Schedule();

	void addCourse(StudyItem* course);

	// polymorphic user task, even though the vector holds generic "StudyItem" pointers the program automatically figures out whether to run the Standard math or the ExamPrep math for each specific item.
	void generateRecommendations() const;
};