#pragma once
#include <string>
#include "ClassSchedule.h"

class FileHandler { //separates file persistence and parsing error handling from GUI and Schedule
public:
	static void saveSchedule(const Schedule& schedule, const std::string& filename); //writes courses (via operator<<) and calendar notes to CSV
	static void loadSchedule(Schedule& schedule, const std::string& filename); //validates into temp buffers so failed loads preserve current data
};