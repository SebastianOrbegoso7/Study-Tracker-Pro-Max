#include <QApplication>
#include "MainWindow.h"

int main(int argc, char* argv[]) {
    // 1. Initialize the Qt visual framework
    QApplication app(argc, argv);

    // 2. Create the window that was designed
    MainWindow window;

    // 3. Make the window visible on the screen
    window.show();

    // 4. Hand over control to Qt so it can look for button clicks
    return app.exec();
}