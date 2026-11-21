#include <iostream>
#include <stdexcept> // throws invalid_argument on bad input or credit limit overflow
#include "ClassSchedule.h"
#include "StandardCourse.h" // used by addCourse Overload 2
#include "Validation.h" // validates weeklyLimit using double template
using namespace std;

Schedule::~Schedule() {
	clear(); // reuses clear() so destructor deletes every owned StudyItem* pointer cleanly
}

void Schedule::clear() { // frees all owned heap pointers and resets calendar notes
	for (size_t i = 0; i < courseList.size(); i++) {
		delete courseList[i];
	}
	courseList.clear();
	calendarNotes.clear();
}

int Schedule::getTotalCredits() const { // sums credit hours across all courses
	int total = 0;
	for (size_t i = 0; i < courseList.size(); i++) {
		total += courseList[i]->getCredits();
	}
	return total;
}

void Schedule::addCourse(StudyItem* course) { // Overload 1: enforces 18-credit cap without leaking memory
	if (course == nullptr) {
		throw invalid_argument("Cannot add a null course.");
	}
	if (getTotalCredits() + course->getCredits() > MAX_TOTAL_CREDITS) {
		int attempted = getTotalCredits() + course->getCredits();
		delete course; // frees rejected pointer before throwing to prevent memory leak
		throw invalid_argument("Total credit hours (" + to_string(attempted) + ") cannot exceed 18.");
	}
	courseList.push_back(course);
}

void Schedule::addCourse(const string& name, int credits, int difficulty) { // Overload 2: constructs StandardCourse and delegates to Overload 1
	addCourse(new StandardCourse(name, credits, difficulty));
}

bool Schedule::removeCourse(size_t index) { // deletes one selected course and frees its heap memory
	if (index >= courseList.size()) {
		return false;
	}
	delete courseList[index];
	courseList.erase(courseList.begin() + index);
	return true;
}

StudyPlanResult Schedule::buildRecommendations(double weeklyLimit) const { // polymorphic calculation + proportional scaling around user's max weekly study time
	validateRange<double>(weeklyLimit, 1.0, 168.0, "Weekly study limit"); // template instantiation with double (rejects -2, 0, >168)
	if (courseList.empty()) {
		throw invalid_argument("Schedule cannot be empty. Add at least one class first.");
	}

	StudyPlanResult result;
	result.weeklyLimit = weeklyLimit;

	for (size_t i = 0; i < courseList.size(); i++) {
		double ideal = 0.0;
		courseList[i]->calculatestudyhours(ideal); // runtime polymorphic dispatch to StandardCourse or ExamPrep
		result.entries.push_back({ courseList[i]->getName(), courseList[i]->getType(), ideal, ideal });
		result.totalIdealHours += ideal;
	}

	if (result.totalIdealHours > weeklyLimit && result.totalIdealHours > 0.0) { // scales each class proportionally to fit max weekly limit
		double scaleFactor = weeklyLimit / result.totalIdealHours;
		result.wasScaled = true;
		for (auto& entry : result.entries) {
			entry.scaledHours = entry.idealHours * scaleFactor;
			result.totalScheduledHours += entry.scaledHours;
		}
	}
	else {
		result.totalScheduledHours = result.totalIdealHours;
	}

	return result;
}

void Schedule::generateRecommendations(double weeklyLimit) const {
	StudyPlanResult plan = buildRecommendations(weeklyLimit); // delegates console version to shared backend math
	cout << "\n--- Study Recommendations ---\n";
	for (const auto& entry : plan.entries) {
		cout << entry.courseName << " (" << entry.courseType << "): " << entry.scaledHours << " hrs/week\n";
	}
	cout << "Total Ideal: " << plan.totalIdealHours << " | Recommended: " << plan.totalScheduledHours << " / " << plan.weeklyLimit << " hrs\n";
}

void Schedule::setCalendarNote(const string& dateISO, const string& note) { // stores or clears a calendar day note
	size_t first = note.find_first_not_of(" \t\r\n");
	if (first == string::npos) {
		calendarNotes.erase(dateISO);
	}
	else {
		calendarNotes[dateISO] = note;
	}
}

string Schedule::getCalendarNote(const string& dateISO) const { // returns saved note or empty string
	auto it = calendarNotes.find(dateISO);
	return (it != calendarNotes.end()) ? it->second : "";
}

void Schedule::replaceAll(vector<StudyItem*>& newCourses, const map<string, string>& newNotes) { // replaces data only after full file load succeeds
	clear();
	courseList.swap(newCourses);
	calendarNotes = newNotes;
}