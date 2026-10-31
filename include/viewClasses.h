#pragma once         // Tells C++ to load this file only once
#include "StudyItem.h"
#include <vector>    // Allows us to use std::vector (a list)
#include <string>    // Allows us to use std::string (text)
using namespace std;

class viewClass{
public:
	void viewEnteredClasses(const vector<StudyItem*>&classes) const;
};
