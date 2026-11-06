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
	int total_credits = 0; // ADDED: total_credits variable to keep track of total credits that the user has entered into their schedule.
	// Basic loops here
	do { // Added switch case
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

		switch (choice) { // ADDED: switch case statement instead of multiple if-else conditional statements
		case 1: { // ADDED: A set of curly braces so the "string name" line doesn't bypass any implementation and not run into errors.
			do { // ADDED: A do-while loop to stop accepting more than 18 credit hours.
				int courseType;
				string name;
				int credits;
				cout << "Enter course type (1 for standard, 2 for exam prep): ";
				cin >> courseType;

				// FIX: Clear the leftover newline from the buffer before using getline
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				// Now allows for correct course input
				do { // ADDED: A do-while loop to keep asking if user enters an invalid course name.
					cout << "Enter course name: ";
					// FIX: Use getline to allow spaces in the course name
					getline(cin, name);

					if (name.length() > 8 || name.length() < 7) { // ADDED: An if conditional statement to let the user know that they've entered an invalid course name and that it should be 7-8 characters long with the format of "XXX 1234" or "XXX1234" (not case-sensitive).
						cout << "Course name must either be 7 or 8 characters long with formats of \"XXX 1234\", \"XXX1234\", \"xxx 1234\", or \"xxx1234\"." << endl;
					}
				} while (name.length() > 8 || name.length() < 7); // Expression to make sure the loop repeats if the user enters an invalid course name.

				do { // ADDED: A do-while loop to repeat until user enters a valid number of credit hours
					cout << "Enter credit hours: ";
					cin >> credits;

					if (credits <= 4 || credits >= 1) {
						total_credits += credits; // Sum up all the credits
					}

					if (credits > 4 || credits < 1) {
						cout << "You've entered an invalid number of credit hours. Please try again." << endl; // Ask the user again if they enter an invalid number of credit hours
					}
				} while (credits > 4 || credits < 1);



				if (courseType == 1) {
					int difficulty;
					cout << "Enter difficulty: " << endl;
					cin >> difficulty;

					if (total_credits <= 18) { // ADDED: An if conditional statement to not accept any more classes if the total credit hours will exceed 18.
						mySchedule.addCourse(new StandardCourse(name, credits, difficulty));
						cout << name << " added to schedule." << endl;
					}
				}
				else if (courseType == 2) {
					int days;
					cout << "Enter days until exam: ";
					cin >> days;

					if (total_credits <= 18) { // ADDED: An if conditional statement to not accept any more classes if the total credit hours will exceed 18.
						mySchedule.addCourse(new ExamPrep(name, credits, days));
						cout << name << " added as urgent test prep." << endl;
					}
				}
				else {
					std::cout << "Invalid course type selected." << endl;
				}
			} while (total_credits < 18 && total_credits < 0); // total_credits had to be included inside the expression with && so it the loop doesn't repeat in the same case when the user want to enter another choice in the menu.

			if (total_credits > 18) { // ADDED: An if conditional statement to let the user know that they have exceeded the maximum number of credit hours.
				std::cout << "You have exceeded the maximum number of credit hours." << endl;
			}
			break;
		}
		case 2:
			try {
				double weeklyLimit;

				cout << "\nEnter your maximum study hours per week: ";
				cin >> weeklyLimit;

				while (cin.fail() || weeklyLimit <= 0) {
					cin.clear();
					cin.ignore(10000, '\n');

					cout << "Invalid input. Enter a number greater than 0: ";
					cin >> weeklyLimit;
				}

				mySchedule.generateRecommendations(weeklyLimit);
			}
			catch (const std::runtime_error& e) {
				cout << "\nApplication Error: " << e.what() << "\n"; // e.what() error message
				cout << "Your data is preserved. Please try again.\n";
			}
			break;
		case 3:
			// Instantiate the viewer and pass it to the schedule's list
			viewClass viewer;
			viewer.viewEnteredClasses(mySchedule.getCourseList());
			break;
		case 4:
			cout << "Exiting program. Good Luck studying!\n" << endl;
			break;
		default:
			cout << "Invalid choice. Please pick 1, 2, 3, or 4.\n" << endl;
		}
	} while (choice != 4);
	return 0;
}
// Right now, it prints recommended time per week, soon we will add a calander feature and make it track more reasonably 