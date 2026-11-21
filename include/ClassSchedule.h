#pragma once
#include <vector>
#include <string> // needed for calendar notes and recommendation structs
#include <map> // stores date-to-note mapping in backend (no Qt dependency)
#include "StudyItem.h"
using namespace std;

struct CourseRecommendation { // carries per-course ideal and budget-scaled hours from backend to GUI
	string courseName;
	string courseType;
	double idealHours;
	double scaledHours;
};

struct StudyPlanResult { // full recommendation summary returned by Schedule::buildRecommendations
	vector<CourseRecommendation> entries;
	double totalIdealHours = 0.0;
	double totalScheduledHours = 0.0;
	double weeklyLimit = 0.0;
	bool wasScaled = false;
};

class Schedule {
private:
	vector<StudyItem*> courseList; // vector of pointers to StudyItem objects
	map<string, string> calendarNotes; // stores YYYY-MM-DD calendar notes in backend so FileHandler can persist them
	static const int MAX_TOTAL_CREDITS = 18; // enforces maximum 18 total credit hours per schedule
public:
	Schedule() = default; // explicit default constructor
	Schedule(const Schedule&) = delete; // prevents unsafe shallow copy of owned StudyItem* pointers
	Schedule& operator=(const Schedule&) = delete; // prevents double-free on copy assignment
	~Schedule();

	void addCourse(StudyItem* course); // Overload 1: adds any polymorphic StudyItem* (enforces 18-credit cap)
	void addCourse(const string& name, int credits, int difficulty); // Overload 2: convenience overload for StandardCourse
	bool removeCourse(size_t index); // deletes a single course cleanly and frees its heap memory
	int getTotalCredits() const; // calculates total enrolled credit hours
	void clear(); // deletes all owned StudyItem* pointers and clears calendar notes

	// polymorphic user task, even though the vector holds generic "StudyItem" pointers the program automatically figures out whether to run the Standard math or the ExamPrep math for each specific item.
	StudyPlanResult buildRecommendations(double weeklyLimit) const; // runs polymorphic math and scales hours to fit weeklyLimit
	void generateRecommendations(double weeklyLimit) const;

	const std::vector<StudyItem*>& getCourseList() const {
		return courseList;
	}

	void setCalendarNote(const string& dateISO, const string& note); // saves or removes a note for a calendar date
	string getCalendarNote(const string& dateISO) const; // retrieves note for a calendar date
	const map<string, string>& getCalendarNotes() const { return calendarNotes; } // exposes notes for UI highlighting & CSV saving
	void replaceAll(vector<StudyItem*>& newCourses, const map<string, string>& newNotes); // atomic swap used by FileHandler after validation passes
};