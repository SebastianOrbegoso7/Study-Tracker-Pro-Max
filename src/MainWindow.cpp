#include "MainWindow.h"
#include "StandardCourse.h"
#include "ExamPrep.h" // new: included examprep so the gui can instantiate it
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <stdexcept>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	setWindowTitle("Study Tracker Pro Max - Checkpoint 7");
	resize(450, 700); // new: made window taller to fit all the new inputs
	QWidget* central = new QWidget(this); //QMainWindow requires a "central widget" to act as a base for all other items.
	//QVboxlayout automatically stacks items vertically, so we don't have to manually set the position of each item.
	QVBoxLayout* layout = new QVBoxLayout(central);

	// new: task 1 - course type dropdown (standard vs exam)
	layout->addWidget(new QLabel("Course Type:"));
	courseTypeInput = new QComboBox();
	courseTypeInput->addItem("Standard Course");
	courseTypeInput->addItem("Exam Prep");
	layout->addWidget(courseTypeInput);

	// user input area
	//QLabel acts as uneditable text on the screen for instruction
	layout->addWidget(new QLabel("Enter course name:"));
	nameInput = new QLineEdit();
	layout->addWidget(nameInput);
	layout->addWidget(new QLabel("Enter credit hours:"));
	creditsInput = new QSpinBox();
	creditsInput->setRange(1, 4); // Set the range of credit hours
	layout->addWidget(creditsInput);
	layout->addWidget(new QLabel("Difficulty (1-5 for Standard):"));
	difficultyInput = new QSpinBox();
	//1-5 restriction
	//qt will prevent the user from entering invalid values, so we don't have to check for invalid input in the code.
	difficultyInput->setRange(1, 5);
	layout->addWidget(difficultyInput);

	// new: task 1 - exam prep days input
	layout->addWidget(new QLabel("Days Until Exam (For Exam Prep):"));
	daysInput = new QSpinBox();
	daysInput->setRange(1, 365); // Prevent 0 or negative days
	layout->addWidget(daysInput);

	//action button
	//submit data button
	addButton = new QPushButton("Add Class");
	layout->addWidget(addButton);

	// new: task 3 - view schedule button
	QPushButton* viewCurrentScheduleBtn = new QPushButton("View Current Schedule");
	layout->addWidget(viewCurrentScheduleBtn);

	//display area
	scheduleDisplay = new QListWidget();
	layout->addWidget(scheduleDisplay);

	// new: task 2 - weekly limit input for recommendations (marlon's feature)
	layout->addWidget(new QLabel("Weekly Study Limit (Hours):"));
	weeklyLimitInput = new QDoubleSpinBox();
	weeklyLimitInput->setRange(1.0, 168.0); // Max hours in a week
	layout->addWidget(weeklyLimitInput);

	// Button to generate recommendations
	generateRecommendationsBtn = new QPushButton("Generate Recommendations");
	layout->addWidget(generateRecommendationsBtn);

	// Area to view recommended study times
	generatedRecommendations = new QListWidget();
	layout->addWidget(generatedRecommendations);

	// new: exit button
	QPushButton* exitBtn = new QPushButton("Exit");
	layout->addWidget(exitBtn);

	//connect the button to the function that will handle adding a class
	setCentralWidget(central);
	//tells qt when add buttons clicked, call the handleAddClass function in this class.
	connect(addButton, &QPushButton::clicked, this, &MainWindow::handleAddClass);

	// new: wiring up the view schedule, generate, and exit buttons
	connect(viewCurrentScheduleBtn, &QPushButton::clicked, this, &MainWindow::handleViewSchedule);
	connect(generateRecommendationsBtn, &QPushButton::clicked, this, &MainWindow::handleGenerateRecommendations);
	connect(exitBtn, &QPushButton::clicked, this, &QWidget::close);
}

//GUI event handler for adding a class
void MainWindow::handleAddClass() {
	//extract data from the input fields
	int credits = creditsInput->value();
	int difficulty = difficultyInput->value();
	int days = daysInput->value(); // new: extracting the days value for exam prep

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

		// new: polymorphism in action - checking the combo box
		if (courseTypeInput->currentIndex() == 0) {
			// Call the application function: Instantiate the exact same polymorphic class 
			// you used in the console version, and add it to your Schedule object.
			mySchedule.addCourse(new StandardCourse(name, credits, difficulty));
		}
		else {
			mySchedule.addCourse(new ExamPrep(name, credits, days));
		}

		// Display the visible result
		// If successful, create a new text entry in the display box.
		// We use QString::fromStdString() to convert your C++ string back into Qt's format.
		QMessageBox::information(this, "Success", QString::fromStdString(name + " added successfully.")); // new: changed to qmessagebox::information so it looks like a success pop-up, not a warning

		// Reset the input boxes for the next class entry
		nameInput->clear();
		creditsInput->setValue(1);
		difficultyInput->setValue(1);
		daysInput->setValue(1); // new: resetting the days input field

		handleViewSchedule(); // new: auto-refresh the view schedule list so the user sees it immediately

	}
	catch (const std::exception& e) {
		//  Handle Invalid Input (The final Checkpoint Requirement)
		// If the try block throws an error, this creates a pop-up warning box 
		// instead of crashing the program. 
		// e.what() prints the exact string we wrote in the throw statement above.
		QMessageBox::warning(this, "Application Error", e.what());
	}
}

// new: entire function added for task 3 (view schedule)
void MainWindow::handleViewSchedule() {
	scheduleDisplay->clear();

	const auto& courses = mySchedule.getCourseList();
	if (courses.empty()) {
		scheduleDisplay->addItem("No classes entered yet.");
		return;
	}

	for (StudyItem* item : courses) {
		QString info = QString::fromStdString(item->getName()) + " | Credits: " + QString::number(item->getCredits());
		scheduleDisplay->addItem(info);
	}
}

void MainWindow::handleGenerateRecommendations() { // Added: class to generate recommended study hours.
	try {
		// new: moved this check to the top. if it's empty, we should throw the error before doing any math.
		if (mySchedule.getCourseList().empty()) {
			throw std::invalid_argument("Schedule cannot be empty."); // Message to throw in the error pop-up.
		}

		generatedRecommendations->clear(); // Clear the output area for new classes when the program re-opens.

		double totalRecommendedHours = 0.0; // new: variables to track total hours and grab the weekly limit
		double weeklyLimit = weeklyLimitInput->value(); // new: variables to track total hours and grab the weekly limit

		for (StudyItem* item : mySchedule.getCourseList()) { // Iterate through all classes in the schedule.
			double classHours = 0.0;
			item->calculatestudyhours(classHours); // Study hours is calculated with the formula (credits * 2) * difficulty / 3.

			totalRecommendedHours += classHours; // new: tallying up total hours for the warning logic

			QString studyRecommendation = QString::fromStdString(item->getName()) + ": " + QString::number(classHours) + " hours/week."; // Print the number of hours the user should study for the class.
			generatedRecommendations->addItem(studyRecommendation); // Output the result in the appropriate list widget (the second to last box in the main GUI).
		}

		// new: adding the totals and marlon's visual warning logic to the gui
		generatedRecommendations->addItem("----------------------------------");
		generatedRecommendations->addItem("Total Required: " + QString::number(totalRecommendedHours) + " hrs");
		generatedRecommendations->addItem("Your Limit: " + QString::number(weeklyLimit) + " hrs");

		if (totalRecommendedHours > weeklyLimit) {
			QMessageBox::warning(this, "Schedule Warning",
				"WARNING: Your recommended study hours exceed your weekly limit!\nConsider adjusting your schedule.");
		}
		else {
			QMessageBox::information(this, "Schedule Good",
				"Your study recommendations are within your weekly limit!");
		}
	}
	catch (const std::exception& e) {
		QMessageBox::warning(this, "Application Error", e.what()); // The error pop-up will let the user know that the schedule cannot be empty.
	}
}