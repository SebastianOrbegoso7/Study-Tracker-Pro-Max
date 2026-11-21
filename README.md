# Study Tracker Pro Max

## Environment & Build/Run
* **OS:** Windows 11 (64-bit)
* **IDE:** Visual Studio 2026 (v18.12)
* **Compiler:** MSVC 2026 (v145 / Build Tools 14.52, 64-bit)
* **C++ Standard:** C++17 (`/EHsc`)
* **Framework:** QtWidgets (`msvc2022_64` kit), CMake 3.16+

1. Clone the repository and open the root folder (`CMakeLists.txt`) in Visual Studio 2026.
2. Let CMake configure (if Qt is not auto-detected, set `CMAKE_PREFIX_PATH` to your Qt kit folder, e.g., `C:/Qt/6.x.x/msvc2022_64`).
3. Build and run the `MyQtProject` target.

## AI Use Disclosure
We used **Google Gemini** for:
* Qt 6 GUI setup and fixing `QCalendarWidget` because we couldn't get it to work seamlessly.
* CSV persistence and malformed-file rollback logic in `FileHandler`.
* `CMakeLists.txt` setup (`/EHsc`, Qt auto-detection, and `windeployqt`).
* Debugging the `ExamPrep` negative-hours bug and refactoring validation/study-hour scaling.
* * Simple logic fixes (preventing `std::stoi` crashes on non-numeric course codes and keeping list refreshes separate from opening the calendar).
* Checking memory cleanup on rejected course additions and reviewing the project against the COP 3003 checklist.
