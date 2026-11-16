#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox> // new: for weekly study limit
#include <QPushButton>
#include <QListWidget>
#include <QComboBox>      // new: for course type dropdown
#include "ClassSchedule.h"

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);
private slots:
	void handleAddClass();
	void handleGenerateRecommendations();
	void handleViewSchedule(); // new: event handler for task 3
private:
	QComboBox* courseTypeInput; // new: dropdown for course type
	QLineEdit* nameInput;
	QSpinBox* creditsInput;
	QSpinBox* difficultyInput;
	QSpinBox* daysInput;        // new: for exam prep days
	QDoubleSpinBox* weeklyLimitInput; // new: for weekly limit
	QPushButton* addButton;
	QListWidget* scheduleDisplay;
	Schedule mySchedule;
	QPushButton* generateRecommendationsBtn;
	QListWidget* generatedRecommendations;
};