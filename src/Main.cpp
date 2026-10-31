#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "ClassSchedule.h"
#include "StandardCourse.h"
#include "ExamPrep.h"
#include "viewClasses.h"
#include <QWidget>
using namespace std;


int main() { 
	Schedule mySchedule;
	// Console driver here:
	cout << "W.I.P. Study Tracker V2" << endl; // Fixed grammatical error: uncapitalized title
	int choice = 0;
	// Basic loops here
	while (choice != 4) { // Add switch case later
		cout << "Menu: " << endl;// In the future for this, menu I want to have a total tracked # hours
		cout << "1. Add Class" << endl; // Fixed grammatical error: uncapitalized option title
		cout << "2. Generate Recommendations" << endl; // Fixed grammatical error: uncapitalized option title
		cout << "3. View Current Schedule" << endl; // Fixed grammatical error: uncapitalized option title
		cout << "4. Exit" << endl; // Fixed grammatical error: uncapitalized option title
		cout << "Enter Choice: " << endl; // Fixed grammatical error: uncapitalized option title
		cin >> choice;
		if (cin.fail()) { // Check for invalid input
			cin.clear(); // Clear the error flag
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
			cout << "Invalid input. Please enter a number." << endl;
			continue; // Go back to the start of the loop
		}

		if (choice == 1) {
			int courseType;
			string name;
			int credits;
			cout << "Enter course type (1 for standard, 2 for exam prep): ";
			cin >> courseType;

			// FIX: Clear the leftover newline from the buffer before using getline
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			// now allows for correct course input
			cout << "Enter course name: ";
			// FIX: Use getline to allow spaces in the course name
			getline(cin, name);

			cout << "Enter credit hours: ";
			cin >> credits;

			if (courseType == 1) {
				int difficulty;
				cout << "Enter difficulty: " << endl;
				cin >> difficulty;

				mySchedule.addCourse(new StandardCourse(name, credits, difficulty));
				cout << name << " added to schedule." << endl;
			}
			else if (courseType == 2) {
				int days;
				cout << "Enter days until exam: ";
				cin >> days;

				mySchedule.addCourse(new ExamPrep(name, credits, days));
				cout << name << " added as urgent test prep." << endl;
			}
			else {
				std::cout << "Invalid course type selected." << endl;
			}

		}
		else if (choice == 2) {
			try {
				mySchedule.generateRecommendations();// checkpoint requirment !!

			}
			catch (const std::runtime_error& e) {
				cout << "\nApplication Error: " << e.what() << "\n"; // e.what() error message 
				cout << "Your data is preserved. Please try again.\n";
			}
		}
		else if (choice == 3) {
			//Instantiate the viewer and pass it the schedule's list
			viewClass viewer;
			viewer.viewEnteredClasses(mySchedule.getCourseList());
		}
		else if (choice == 4) {
			cout << "Exiting program. Good Luck Studying!\n" << endl;
		}
		else {
			cout << "Invalid choice. Please pick 1, 2, 3, or 4.\n" << endl;
		}
	}
	return 0;
}
// Right now, it prints recommended time per week, soon we will add a calander feature and make it track more reasonably 