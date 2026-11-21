#pragma once
#include <string>
#include <stdexcept> // needed for std::invalid_argument exception handling
#include <cctype> // needed for std::isalpha and std::isdigit format checks

// Template function instantiated with two data types: int (credits/difficulty/days) and double (weeklyLimit)
template <typename T>
inline void validateRange(T value, T minVal, T maxVal, const std::string& fieldName) { // reusable numeric bounds check
	if (value < minVal || value > maxVal) { // throws descriptive error when input is out of bounds
		throw std::invalid_argument(fieldName + " must be between " + std::to_string(minVal) + " and " + std::to_string(maxVal) + ".");
	}
}

inline void validateCourseCode(const std::string& courseCode) { // enforces strict "XXX 1234" format so malformed codes like "COP3003" are rejected
	if (courseCode.empty()) {
		throw std::invalid_argument("Course code cannot be empty.");
	}
	if (courseCode.length() != 8 || courseCode[3] != ' ') { // rejects missing space or wrong length
		throw std::invalid_argument("Course code must follow 'XXX 1234' format (e.g., COP 3003).");
	}
	for (int i = 0; i < 3; i++) { // verifies first 3 characters are letters
		if (!std::isalpha(static_cast<unsigned char>(courseCode[i]))) {
			throw std::invalid_argument("Course prefix must be 3 letters (e.g., COP 3003).");
		}
	}
	for (int i = 4; i < 8; i++) { // verifies last 4 characters are digits before calling std::stoi
		if (!std::isdigit(static_cast<unsigned char>(courseCode[i]))) {
			throw std::invalid_argument("Course number must be 4 digits (e.g., COP 3003).");
		}
	}
	int courseNumber = std::stoi(courseCode.substr(4, 4));
	validateRange<int>(courseNumber, 1000, 4999, "Course number"); // template instantiation with int
}

inline int getCourseDifficulty(const std::string& courseCode) {
	if (courseCode.length() != 8 || courseCode[3] != ' ') {
		return 0;
	}
	for (int i = 4; i < 8; i++) { // prevents std::stoi crash on non-numeric characters
		if (!std::isdigit(static_cast<unsigned char>(courseCode[i]))) {
			return 0;
		}
	}

	int courseNumber = std::stoi(courseCode.substr(4, 4));

	if (courseNumber >= 1000 && courseNumber <= 1999)
		return 1;
	else if (courseNumber >= 2000 && courseNumber <= 2999)
		return 2;
	else if (courseNumber >= 3000 && courseNumber <= 3999)
		return 3;
	else if (courseNumber >= 4000 && courseNumber <= 4999)
		return 4;
	else
		return 0;
}