#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QWidget>

#include "mainwindow.h"
#include "welcomedialog.h"

#include "build_config.h"
#include "rust/cxx.h"
#include "impulse-core/src/bridge.rs.h"
#include "util/notification.h"


namespace {
    // Determine whether to show the welcome dialog or not
    bool shouldWelcomeUser() {
        try{
            if (!impulse_core::resource_check_user_database_exists()) {
                qDebug() << "Did not find an existing user database, so welcome the user";
                return true;
            }
            else {
                qDebug() << "An existing user database was found, but additional checks required.";
                try {
                    auto primary_user = impulse_core::user_get_primary_user();

                    // The SQL query was successful
                    if (impulse_core::user_optional_user_is_some(primary_user)) {
                        qDebug() << "A primary user was found, so do not welcome the user";
                        return false;
                    }
                    else {
                        qDebug() << "No primary user was found, so welcome the user.";
                        return true;
                    }
                }
                catch (const rust::Error& e) {
                    QString getPrimaryUserFailedMessage = (
                        "Failed to get the primary user and cannot continue: "
                        );
                    getPrimaryUserFailedMessage.append(e.what());
                    qCritical().noquote() << getPrimaryUserFailedMessage;
                    util::notify::critical(nullptr, "Setup Failed", getPrimaryUserFailedMessage);
                    std::abort();
                }
            }
        }
        catch (const rust::Error& e) {
            QString checkExistenceFailedMessage = (
                "Failed to check if the user database exists and cannot continue: "
            );
            checkExistenceFailedMessage.append(e.what());
            qCritical().noquote() << checkExistenceFailedMessage;
            util::notify::critical(nullptr, "Setup Failed", checkExistenceFailedMessage);
            std::abort();
        }
    }

    /// Get the main stylesheet used by the application
    QString getMainStyleSheet() {
        QString path = ":/styles/main.qss";
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString failedToOpenMessage = QStringLiteral(
                "Failed to open the main stylesheet located at %1: %2"
            ).arg(QString(path), file.errorString());
            qWarning().noquote() << failedToOpenMessage;
            return {};
        }

        return QString::fromUtf8(file.readAll());
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(APP_NAME);
    app.setApplicationDisplayName(APP_DISPLAY_NAME);
    app.setApplicationVersion(APP_VERSION);
    app.setStyleSheet(getMainStyleSheet());

    MainWindow mainWindow;
    WelcomeDialog welcomeDialog(nullptr);

    impulse_core::initialize();

    // Global Signals and Slot Connections
    QObject::connect(&welcomeDialog, &WelcomeDialog::mainWindowRequested, [&]() {
        mainWindow.showMaximized();
    });

    if (shouldWelcomeUser()) {
        welcomeDialog.setFixedSize(welcomeDialog.size());

        // Blocking is okay here as user input is required before showing the main window
        if (welcomeDialog.exec() != QDialog::Accepted) {
            return 0;
        }
    }
    else {
        mainWindow.showMaximized();
    }

    return app.exec();
}
