#pragma once

#pragma once

#include <string>

inline int getCourseDifficulty(const std::string& courseCode) {

    if (courseCode.length() != 8 || courseCode[3] != ' ') {
        return 0;
    }

    int courseNumber = std::stoi(courseCode.substr(4, 4));

    if (courseNumber >= 1000 && courseNumber <= 1999)
        return 1;
    else if (courseNumber >= 2000 && courseNumber <= 2999)
        return 2;
    else if (courseNumber >= 3000 && courseNumber <= 3999)
        return 3;
    else if (courseNumber >= 4000 && courseNumber <= 4999)
        return 4;
    else
        return 0;
}