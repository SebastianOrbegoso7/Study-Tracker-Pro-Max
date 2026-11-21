#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QListWidget>
#include <QComboBox>
#include "ClassSchedule.h"

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);
private slots:
	void handleAddClass();
	void handleGenerateRecommendations();
	void handleViewSchedule();
	void handleLoadFile(); //loads saved or malformed CSV files
private:
	void refreshScheduleList(); //updates class list without opening calendar

	QComboBox* courseTypeInput;
	QLineEdit* nameInput;
	QSpinBox* creditsInput;
	QSpinBox* difficultyInput;
	QSpinBox* daysInput;
	QDoubleSpinBox* weeklyLimitInput;
	QListWidget* scheduleDisplay;
	QListWidget* generatedRecommendations;
	Schedule mySchedule;
};