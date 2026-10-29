#pragma once
#include "StudyItem.h"
#include <string>
using namespace std;

class StandardCourse : public StudyItem { //first derived class
private:
	int difficulty; // for our difficulty level track

public:
	StandardCourse(string n, int c, int diff) : StudyItem(n, c), difficulty(diff) {} // constructor

	void calculatestudyhours() const override;
};