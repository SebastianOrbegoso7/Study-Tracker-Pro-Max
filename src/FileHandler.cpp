#include "FileHandler.h"
#include "StandardCourse.h"
#include "ExamPrep.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <map>
#include <cctype>

namespace {
	int parseStrictInt(const std::string& text, const std::string& fieldName) { // rejects malformed numbers like "3abc"
		if (text.empty()) throw std::runtime_error("Missing integer for " + fieldName + ".");
		size_t start = (text[0] == '-') ? 1 : 0;
		if (start == text.size()) throw std::runtime_error("Invalid integer for " + fieldName + ".");
		for (size_t i = start; i < text.size(); i++) {
			if (!std::isdigit(static_cast<unsigned char>(text[i]))) {
				throw std::runtime_error("Invalid integer '" + text + "' for " + fieldName + ".");
			}
		}
		return std::stoi(text);
	}

	std::string escapeNote(const std::string& raw) { // keeps multi-line calendar notes on one CSV row
		std::string out;
		for (char c : raw) {
			if (c == '\\') out += "\\\\";
			else if (c == '\n') out += "\\n";
			else if (c != '\r') out += c;
		}
		return out;
	}

	std::string unescapeNote(const std::string& encoded) { // restores line breaks in loaded calendar notes
		std::string out;
		for (size_t i = 0; i < encoded.size(); i++) {
			if (encoded[i] == '\\' && i + 1 < encoded.size()) {
				if (encoded[i + 1] == 'n') { out += '\n'; i++; }
				else if (encoded[i + 1] == '\\') { out += '\\'; i++; }
				else out += encoded[i];
			}
			else {
				out += encoded[i];
			}
		}
		return out;
	}
}

void FileHandler::saveSchedule(const Schedule& schedule, const std::string& filename) { // persists schedule and calendar notes to CSV
	std::ofstream out(filename);
	if (!out.is_open()) {
		throw std::runtime_error("Could not open file for saving: " + filename);
	}

	out << "TYPE,NAME_OR_DATE,CREDITS_OR_NOTE,EXTRA\n";
	for (const StudyItem* item : schedule.getCourseList()) {
		out << *item << "\n"; // invokes overloaded operator<< on StudyItem
	}
	for (const auto& entry : schedule.getCalendarNotes()) {
		out << "NOTE," << entry.first << "," << escapeNote(entry.second) << "\n"; // persists calendar day notes
	}
	if (!out.good()) {
		throw std::runtime_error("Write error while saving schedule.");
	}
}

void FileHandler::loadSchedule(Schedule& schedule, const std::string& filename) { // stages parsed rows so malformed files preserve existing data
	std::ifstream in(filename);
	if (!in.is_open()) {
		throw std::runtime_error("Could not open file: " + filename + " (existing schedule preserved).");
	}

	std::vector<StudyItem*> tempCourses;
	std::map<std::string, std::string> tempNotes;
	int tempCredits = 0;
	std::string line;
	int lineNum = 0;

	try {
		while (std::getline(in, line)) {
			lineNum++;
			if (!line.empty() && line.back() == '\r') line.pop_back();
			if (line.empty()) continue;
			if (lineNum == 1 && line == "TYPE,NAME_OR_DATE,CREDITS_OR_NOTE,EXTRA") continue;

			std::stringstream ss(line);
			std::string type;
			std::getline(ss, type, ',');

			if (type == "STANDARD" || type == "EXAMPREP") {
				std::string name, creditsStr, extraStr, trailing;
				if (!std::getline(ss, name, ',') || !std::getline(ss, creditsStr, ',') || !std::getline(ss, extraStr, ',')) {
					throw std::runtime_error("Missing columns on line " + std::to_string(lineNum) + ".");
				}
				if (std::getline(ss, trailing)) {
					throw std::runtime_error("Unexpected extra data on line " + std::to_string(lineNum) + ".");
				}

				int credits = parseStrictInt(creditsStr, "credits");
				int extra = parseStrictInt(extraStr, (type == "STANDARD") ? "difficulty" : "daysUntilExam");
				if (tempCredits + credits > 18) {
					throw std::runtime_error("Loaded courses exceed the 18-credit limit on line " + std::to_string(lineNum) + ".");
				}

				StudyItem* item = (type == "STANDARD")
					? static_cast<StudyItem*>(new StandardCourse(name, credits, extra))
					: static_cast<StudyItem*>(new ExamPrep(name, credits, extra));
				tempCourses.push_back(item);
				tempCredits += credits;
			}
			else if (type == "NOTE") {
				std::string dateISO, noteText;
				if (!std::getline(ss, dateISO, ',') || !std::getline(ss, noteText) || dateISO.length() != 10) {
					throw std::runtime_error("Malformed calendar note on line " + std::to_string(lineNum) + ".");
				}
				tempNotes[dateISO] = unescapeNote(noteText);
			}
			else {
				throw std::runtime_error("Unknown entry type '" + type + "' on line " + std::to_string(lineNum) + ".");
			}
		}
	}
	catch (const std::exception& e) { // rolls back staged pointers on any error so valid in-memory data stays untouched
		for (StudyItem* ptr : tempCourses) {
			delete ptr;
		}
		throw std::runtime_error(std::string("File load aborted (existing schedule preserved): ") + e.what());
	}

	schedule.replaceAll(tempCourses, tempNotes); // commits validated data atomically
}