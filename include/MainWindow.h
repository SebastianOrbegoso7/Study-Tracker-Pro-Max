#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QListWidget>
#include "ClassSchedule.h"

class MainWindow: public QMainWindow {
	Q_OBJECT
public:
	MainWindow(QWidget* parent = nullptr);
private slots: 
	void handleAddClass(); // gui eent handler for the task of adding a class
	void handleGenerateRecommendations(); // Event handler to generate recommendations
private: 
	QLineEdit* nameInput;
	QSpinBox* creditsInput;
	QSpinBox* difficultyInput;
	QPushButton* addButton;
	QListWidget* scheduleDisplay; // result here
	Schedule mySchedule;
	QPushButton* generateRecommendationsBtn;
	QListWidget* generatedRecommendations;
};