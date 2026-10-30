#include <iostream>
#include <vector>
#include <string>
#include "viewClasses.h" // 1. Include the header file at the top

int main() {
    std::vector<std::string> myClasses; // Holds all class names entered by the user
    int choice = 3;                     // Example user choice variable

    // Using if, else if, and else statements to handle menu choices
    if (choice == 1) {
        // Code to add a class would go here
        std::cout << "Option 1 chosen: Add class\n";
    }
    else if (choice == 2) {
        // Other menu logic would go here
        std::cout << "Option 2 chosen\n";
    }
    else if (choice == 3) {
        // 2. CALL THE FUNCTION HERE! 
        // Pass your 'myClasses' list into it so it prints out the entered classes.
        viewEnteredClasses(myClasses);
    }
    else {
        // Catches any invalid numbers entered by the user
        std::cout << "Invalid choice! Please try again.\n";
    }

    return 0; 