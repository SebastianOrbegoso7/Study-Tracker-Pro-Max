#include "MainWindow.h"
#include "StandardCourse.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <stdexcept>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	setWindowTitle("Study Tracker Pro Max - W.I.P v3");
	resize(400, 300);
	QWidget* central = new QWidget(this); //QMainWindow requires a "central widget" to act as a base for all other items.
	//QVboxlayout automatically stacks items vertically, so we don't have to manually set the position of each item.
	QVBoxLayout* layout = new QVBoxLayout(central);
	// user input area
	//QLabel acts as uneditable text on the screen for instruction
	layout->addWidget(new QLabel("Enter course name:"));
	nameInput = new QLineEdit();
	layout->addWidget(nameInput);
	layout->addWidget(new QLabel("Enter credit hours:"));
	creditsInput = new QSpinBox();
	creditsInput->setRange(1, 4); // Set the range of credit hours
	layout->addWidget(creditsInput);
	layout->addWidget(new QLabel("Difficulty (1-5):"));
	difficultyInput = new QSpinBox();
	//1-5 restriction
	//qt will prevent the user from entering invalid values, so we don't have to check for invalid input in the code.
	difficultyInput->setRange(1, 5);
	layout->addWidget(difficultyInput);
	//action button
	//submit data button
	addButton = new QPushButton("Add Class");
	layout->addWidget(addButton);

	// Button to generate recommendations
	QPushButton* generateRecommendationsBtn = new QPushButton("Generate Recommendations");
	layout->addWidget(generateRecommendationsBtn);

	// Button to view current schedule
	QPushButton* viewCurrentScheduleBtn = new QPushButton("View Current Schedule");
	layout->addWidget(viewCurrentScheduleBtn);

	// Button to exit the program
	QPushButton* exitBtn = new QPushButton("Exit");
	layout->addWidget(exitBtn);

	//display area
	scheduleDisplay = new QListWidget();
	layout->addWidget(scheduleDisplay);
	//connect the button to the function that will handle adding a class
	setCentralWidget(central);
	//tells qt when add buttons clicked, call the handleAddClass function in this class.
	connect(addButton, &QPushButton::clicked, this, &MainWindow::handleAddClass);

	// Area to view recommended study times
	generatedRecommendations = new QListWidget();
	layout->addWidget(generatedRecommendations);
	// Event where user clicks on the "Generate Recommendations" button.
	connect(generateRecommendationsBtn, &QPushButton::clicked, this, &MainWindow::handleGenerateRecommendations);
}
//GUI event handler for adding a class
void MainWindow::handleAddClass() {
	//extract data from the input fields
	int credits = creditsInput->value();
	int difficulty = difficultyInput->value();
	//toSTDString is required because Qt uses its own string format, this converts it into a c++ string though
	std::string name = nameInput->text().toStdString();

//pass data to the backend with error handling
	try {
		//name validation so that the user cannot enter an empty string for the name of the class or add a weird name
		if (name.length() < 7 || name.length() > 8) {
			throw std::invalid_argument("Course name must be 7 or 8 characters long\nFormat: 'XXX 1234' or 'XXX1234'.");
		}
		if (credits < 1 || credits > 4) {
			throw std::invalid_argument("Credit hours per class must be between 1 and 4.");
		}
		if (name.empty()) {
			throw std::invalid_argument("Course name cannot be empty.");
		}
		// Call the application function: Instantiate the exact same polymorphic class 
		// you used in the console version, and add it to your Schedule object.
		mySchedule.addCourse(new StandardCourse(name, credits, difficulty));

		// Display the visible result
		// If successful, create a new text entry in the display box.
		// We use QString::fromStdString() to convert your C++ string back into Qt's format.
		scheduleDisplay->addItem(QString::fromStdString(name + " added successfully."));

		// Reset the input boxes for the next class entry
		nameInput->clear();
		creditsInput->setValue(0);
		difficultyInput->setValue(1);

	}
	catch (const std::exception& e) {
		//  Handle Invalid Input (The final Checkpoint Requirement)
		// If the try block throws an error, this creates a pop-up warning box 
		// instead of crashing the program. 
		// e.what() prints the exact string we wrote in the throw statement above.
		QMessageBox::warning(this, "Application Error", e.what());
	}
}
	
void MainWindow::handleGenerateRecommendations() { // Added: class to generate recommended study hours.
	try {
		generatedRecommendations->clear(); // Clear the output area for new classes when the program re-opens.

		for (StudyItem* item : mySchedule.getCourseList()) { // Iterate through all classes in the schedule.
			double recommendedHours = 0.0;

			item->calculatestudyhours(recommendedHours); // Study hours is calculated with the formula (credits * 2) * difficulty / 3.

			QString studyRecommendation = QString::fromStdString(item->getName()) + ": " + QString::number(recommendedHours) + " hours of study time recommended per week."; // Print the number of hours the user should study for the class.

			generatedRecommendations->addItem(studyRecommendation); // Output the result in the appropriate list widget (the second to last box in the main GUI).
		}
		if (mySchedule.getCourseList().empty()) {
			throw std::invalid_argument("Schedule cannot be empty."); // Message to throw in the error pop-up.
		}
	}
	catch (const std::exception& e) {
		QMessageBox::warning(this, "Application Error", e.what()); // The error pop-up will let the user know that the schedule cannot be empty.
	}
}