

#include <iostream>
#include <string>
#include <vector>
#include <limits>
using namespace std;


//base class blueprint
class StudyItem {

	protected: // derived classes CAN access
		string name;
		int CreditHours;

public:
	//constructor
	StudyItem(string n, int c) : name(n), CreditHours(c) {}
	virtual ~StudyItem() {} // A virtual destructor ensures that when a derived class is deleted from memory, itll clean up without junk data
	virtual void calculatestudyhours() const = 0; // =0 makes it a pure virtual class. Thisll force any class that inherits StudyItem to write its own version of this exact function.


};
class StandardCourse : public StudyItem { //first derived class
private:
	int difficulty; // for our difficulty level track

public:
	StandardCourse(string n, int c, int diff) : StudyItem(n, c), difficulty(diff) {} // constructor

	void calculatestudyhours() const override { // override the base class function, this is to ensure a proper replacement of the base class pure virtual
		double hours = (CreditHours * 2.0) * difficulty / 3.0; //credit*2 is for the recommended time to study per credit hour, 3 is just an average is we take difficulty out of 5
		cout << name << " (standard): " << hours << " hours of study time recommended per week." << endl;
	}
};
//derived class 2
class examprep : public StudyItem {  //look into making this into standardcourse class
private:
	int daysUntilExam; // exam prep urgency alg. will start here
public: 
	examprep(string n, int c, int days) : StudyItem(n, c), daysUntilExam(days) {} // constructor again
	// Overriding the math AGAIN, but this time executing the urgency logic for upcoming tests.
	void calculatestudyhours() const override {
		double hours = (CreditHours * 2.0) + ((8 - daysUntilExam) * 1.5);// 8 is used for the urgency logic, an example is 8-1=7so test is one day away, MASSIVE urgency boost, 7 is the urgency SCORE**
		cout << name << " (URGENT): " << hours << " hours/week\n";
	}

};

class schedule {
private:
	vector<StudyItem*> courseList; // vector of pointers to StudyItem objects
public: 
	~schedule() {
		for (size_t i = 0; i < courseList.size(); i++) { // The destructor loops through the vector and deletes the pointers to free up RAM.
			delete courseList[i];
		}
	}
	void addCourse(StudyItem* course) {
		courseList.push_back(course); // adds a new course to the vector
	}

	// polymorphic user task, even though the vector holds generic "StudyItem" pointers the program automatically figures out whether to run the Standard math or the ExamPrep math for each specific item.
	void generaterecommendations() const {

		cout << "\n - - study recommendations - -\n";
		for (size_t i = 0; i < courseList.size(); i++) {
			courseList[i]->calculatestudyhours();
		}
	}
};




int main()
{
	schedule mySchedule;
		//console driver here:
		cout << "W.I.P. study tracker v1" << endl;
	int choice = 0;
	//basic loops here
	while (choice != 3) { // add switch case later
		cout << "Menu: " << endl;// in the future for this menu i want to have a total tracked # hours
		cout << "1. Add class" << endl;
		cout << "2. generate recommendations" << endl;
		cout << " 3. exit" << endl;
		cout << "Enter Choice: "<< endl;
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

				mySchedule.addCourse(new examprep(name, credits, days));
				cout << name << " added as urgent test prep." << endl;
			}
			else {
				std::cout << "Invalid course type selected." << endl;
			}

		}
		else if (choice == 2) {
			mySchedule.generaterecommendations();
		}
		else if (choice != 3) {
			cout << "Invalid choice. Please pick 1, 2, or 3.\n" << endl;
		}
		else {
			cout << "Exiting program. Good Luck Studying!\n" << endl;

		

		}

		


	}


	return 0;
}
//right now it prints recommended time per week, soon we will add a calander feature and make it track more reasonably 