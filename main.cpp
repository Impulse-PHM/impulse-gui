#include "mainwindow.h"

#include <QApplication>

#include "build_config.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName(APP_NAME);
    QApplication::setApplicationDisplayName(APP_DISPLAY_NAME);
    QApplication::setApplicationVersion(APP_VERSION);

    MainWindow main_window;
    main_window.show();
    return app.exec();
}
