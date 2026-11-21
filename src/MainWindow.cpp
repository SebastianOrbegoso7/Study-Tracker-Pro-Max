#include "MainWindow.h"
#include "ExamPrep.h"
#include "Validation.h" // connects automatic course difficulty
#include "FileHandler.h" // connects CSV save/load persistence
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>
#include <QCalendarWidget>
#include <QDialog>
#include <QTextEdit>
#include <QTextCharFormat>
#include <QDate>
#include <QFileDialog> // lets user pick CSV files to test loading/errors
#include <fstream>
#include <stdexcept>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
	setWindowTitle("Study Tracker Pro Max");
	resize(450, 720);
	QWidget* central = new QWidget(this);
	QVBoxLayout* layout = new QVBoxLayout(central);

	layout->addWidget(new QLabel("Course Type:"));
	courseTypeInput = new QComboBox();
	courseTypeInput->addItem("Standard Course");
	courseTypeInput->addItem("Exam Prep");
	layout->addWidget(courseTypeInput);

	layout->addWidget(new QLabel("Course Code (e.g., COP 3003):"));
	nameInput = new QLineEdit();
	layout->addWidget(nameInput);

	layout->addWidget(new QLabel("Credit Hours (1-4):"));
	creditsInput = new QSpinBox();
	creditsInput->setRange(1, 4);
	layout->addWidget(creditsInput);

	layout->addWidget(new QLabel("Difficulty (1-5, Standard):"));
	difficultyInput = new QSpinBox();
	difficultyInput->setRange(1, 5);
	layout->addWidget(difficultyInput);

	layout->addWidget(new QLabel("Days Until Exam (Exam Prep):"));
	daysInput = new QSpinBox();
	daysInput->setRange(1, 365);
	layout->addWidget(daysInput);

	QPushButton* addBtn = new QPushButton("Add Class");
	QPushButton* calendarBtn = new QPushButton("Open Calendar / Study Notes");
	layout->addWidget(addBtn);
	layout->addWidget(calendarBtn);

	scheduleDisplay = new QListWidget();
	layout->addWidget(scheduleDisplay);

	layout->addWidget(new QLabel("Weekly Study Limit (Hours):"));
	weeklyLimitInput = new QDoubleSpinBox();
	weeklyLimitInput->setRange(-10.0, 168.0); // allows negative input like -2 so backend rejection can be demonstrated
	weeklyLimitInput->setValue(15.0); // valid default budget
	layout->addWidget(weeklyLimitInput);

	QPushButton* genBtn = new QPushButton("Generate Recommendations");
	layout->addWidget(genBtn);

	generatedRecommendations = new QListWidget();
	layout->addWidget(generatedRecommendations);

	QPushButton* loadBtn = new QPushButton("Load CSV"); // tests loading saved or malformed CSV files
	QPushButton* clearBtn = new QPushButton("Clear Schedule"); // resets schedule and saved CSV
	layout->addWidget(clearBtn);
	QPushButton* exitBtn = new QPushButton("Exit");
	layout->addWidget(loadBtn);
	layout->addWidget(exitBtn);

	setCentralWidget(central);

	connect(nameInput, &QLineEdit::textChanged, this, [=](const QString& text) { // auto-fills difficulty from course number
		int d = getCourseDifficulty(text.trimmed().toStdString());
		if (d >= 1 && d <= 5) difficultyInput->setValue(d);
		});
	connect(addBtn, &QPushButton::clicked, this, &MainWindow::handleAddClass);
	connect(calendarBtn, &QPushButton::clicked, this, &MainWindow::handleViewSchedule);
	connect(genBtn, &QPushButton::clicked, this, &MainWindow::handleGenerateRecommendations);
	connect(loadBtn, &QPushButton::clicked, this, &MainWindow::handleLoadFile);
	connect(exitBtn, &QPushButton::clicked, this, &QWidget::close);
	connect(clearBtn, &QPushButton::clicked, this, [=]() {
		mySchedule.clear();
		try { FileHandler::saveSchedule(mySchedule, "schedule.csv"); }
		catch (const std::exception& e) { QMessageBox::warning(this, "Save Error", e.what()); }
		refreshScheduleList();
		generatedRecommendations->clear();
		});

	if (std::ifstream("schedule.csv").good()) { // auto-loads saved schedule on relaunch
		try { FileHandler::loadSchedule(mySchedule, "schedule.csv"); }
		catch (const std::exception& e) { QMessageBox::warning(this, "Load Error", e.what()); }
	}
	refreshScheduleList();
}

void MainWindow::refreshScheduleList() { // keeps class list synced without popping up the calendar
	scheduleDisplay->clear();
	const auto& courses = mySchedule.getCourseList();
	if (courses.empty()) {
		scheduleDisplay->addItem("No classes entered yet.");
		return;
	}
	for (StudyItem* item : courses) {
		scheduleDisplay->addItem(QString::fromStdString(item->getName() + " (" + item->getType() + ") | Credits: ") + QString::number(item->getCredits()));
	}
	scheduleDisplay->addItem("Total Credits: " + QString::number(mySchedule.getTotalCredits()) + " / 18");
}

void MainWindow::handleAddClass() {
	std::string name = nameInput->text().trimmed().toStdString();
	try { // all format, range, and 18-credit validation runs in backend
		if (courseTypeInput->currentIndex() == 0) {
			mySchedule.addCourse(name, creditsInput->value(), difficultyInput->value()); // Overload 2 (StandardCourse)
		}
		else {
			mySchedule.addCourse(new ExamPrep(name, creditsInput->value(), daysInput->value())); // Overload 1 (StudyItem*)
		}
		FileHandler::saveSchedule(mySchedule, "schedule.csv"); // auto-saves on every change
		refreshScheduleList();
		nameInput->clear();
	}
	catch (const std::exception& e) {
		QMessageBox::warning(this, "Application Error", e.what());
	}
}

void MainWindow::handleViewSchedule() {
	QDialog* popup = new QDialog(this);
	popup->setWindowTitle("Student Calendar");
	popup->resize(450, 350);
	popup->setAttribute(Qt::WA_DeleteOnClose);

	QVBoxLayout* popupLayout = new QVBoxLayout(popup);
	QCalendarWidget* cal = new QCalendarWidget(popup);
	QLabel* dayLabel = new QLabel("Notes for " + cal->selectedDate().toString("MMM d, yyyy") + ":", popup);
	QTextEdit* dayNote = new QTextEdit(popup);
	dayNote->setText(QString::fromStdString(mySchedule.getCalendarNote(cal->selectedDate().toString("yyyy-MM-dd").toStdString()))); // loads persisted note
	QPushButton* saveBtn = new QPushButton("Save Note", popup);

	popupLayout->addWidget(cal);
	popupLayout->addWidget(dayLabel);
	popupLayout->addWidget(dayNote);
	popupLayout->addWidget(saveBtn);

	QTextCharFormat highlight;
	highlight.setBackground(QColor("yellow"));
	highlight.setForeground(QColor("black"));
	highlight.setFontWeight(QFont::Bold);

	for (const auto& entry : mySchedule.getCalendarNotes()) {
		if (!entry.second.empty()) { // fixed inverted isEmpty() highlight bug
			QDate d = QDate::fromString(QString::fromStdString(entry.first), "yyyy-MM-dd");
			if (d.isValid()) cal->setDateTextFormat(d, highlight);
		}
	}

	connect(cal, &QCalendarWidget::clicked, popup, [=](const QDate& date) {
		dayLabel->setText("Notes for " + date.toString("MMM d, yyyy") + ":");
		dayNote->setText(QString::fromStdString(mySchedule.getCalendarNote(date.toString("yyyy-MM-dd").toStdString())));
		});

	connect(saveBtn, &QPushButton::clicked, popup, [=]() {
		QDate day = cal->selectedDate();
		QString text = dayNote->toPlainText();
		mySchedule.setCalendarNote(day.toString("yyyy-MM-dd").toStdString(), text.toStdString()); // stores note in backend
		cal->setDateTextFormat(day, text.trimmed().isEmpty() ? QTextCharFormat() : highlight);
		try { FileHandler::saveSchedule(mySchedule, "schedule.csv"); } // persists notes to CSV
		catch (const std::exception& e) { QMessageBox::warning(popup, "Save Error", e.what()); }
		});

	popup->exec();
}

void MainWindow::handleGenerateRecommendations() {
	try {
		StudyPlanResult plan = mySchedule.buildRecommendations(weeklyLimitInput->value()); // polymorphic math + budget scaling in backend
		generatedRecommendations->clear();

		for (const auto& entry : plan.entries) {
			QString line = QString::fromStdString(entry.courseName) + ": " + QString::number(entry.scaledHours, 'f', 2) + " hrs/wk";
			if (plan.wasScaled) line += " (ideal: " + QString::number(entry.idealHours, 'f', 2) + ")";
			generatedRecommendations->addItem(line);
		}
		generatedRecommendations->addItem("Total: " + QString::number(plan.totalScheduledHours, 'f', 2) + " / " + QString::number(plan.weeklyLimit, 'f', 2) + " hrs");

		if (plan.wasScaled) {
			QMessageBox::warning(this, "Limit Exceeded", "Ideal hours exceeded your limit and were scaled to fit.");
		}
	}
	catch (const std::exception& e) {
		QMessageBox::warning(this, "Application Error", e.what());
	}
}

void MainWindow::handleLoadFile() { // loads a CSV file or demonstrates safe rollback on malformed files
	QString path = QFileDialog::getOpenFileName(this, "Load Schedule CSV", "", "CSV Files (*.csv);;All Files (*)");
	if (path.isEmpty()) return;
	try {
		FileHandler::loadSchedule(mySchedule, path.toStdString());
		FileHandler::saveSchedule(mySchedule, "schedule.csv");
		refreshScheduleList();
		generatedRecommendations->clear();
	}
	catch (const std::exception& e) {
		QMessageBox::warning(this, "File Load Error", e.what());
	}
}