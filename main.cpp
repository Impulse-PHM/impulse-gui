#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // TODO: Use the values set in CMakeLists.txt (after making them useable) instead of hard-coding them here
    QApplication::setApplicationName("Impulse PHM");
    QApplication::setApplicationVersion("0.1.0");

    MainWindow main_window;
    main_window.show();
    return app.exec();
}
