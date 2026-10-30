#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "ClassSchedule.h"
#include "StandardCourse.h"
#include "ExamPrep.h"
#include <QWidget>
using namespace std;


int main() { 
	Schedule mySchedule;
	//console driver here:
	cout << "W.I.P. study tracker v1" << endl;
	int choice = 0;
	//basic loops here
	while (choice != 3) { // add switch case later
		cout << "Menu: " << endl;// in the future for this menu i want to have a total tracked # hours
		cout << "1. Add class" << endl;
		cout << "2. generate recommendations" << endl;
		cout << "3. View current schedule" << endl;
		cout << "4. exit" << endl;
		cout << "Enter Choice: " << endl;
		cin >> choice;
		if (cin.fail()) { // check for invalid input
			cin.clear(); // clear the error flag
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
			cout << "Invalid input. Please enter a number." << endl;
			continue; // go back to the start of the loop
		}

		if (choice == 1) {
			int courseType;
			string name;
			int credits;
			cout << "Enter course type (1 for standard, 2 for exam prep): ";
			cin >> courseType;
			cout << "Enter course name: ";
			cin >> name;
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
				mySchedule.generateRecommendations();

			}
			catch (const std::runtime_error& e) {
				cout << "\nApplication Error: " << e.what() << "\n"; // e.what() error message 
				cout << "Your data is preserved. Please try again.\n";
			}
		}
		else if (choice == 3) {
			//mySchedule.viewSchedule();
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
//right now it prints recommended time per week, soon we will add a calander feature and make it track more reasonably 