#include <QFileDialog>

#include "welcomedialog.h"
#include "ui_welcomedialog.h"
#include "newuserdialog.h"
#include "rust/cxx.h"
#include "impulse-core/src/bridge.rs.h"
#include "util/notification.h"


WelcomeDialog::WelcomeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WelcomeDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Welcome");
    this->setAllText();

    connect(ui->newUserButton, &QPushButton::clicked, this, [this] {
        NewUserDialog newUserDialog(this);
        newUserDialog.setFixedSize(newUserDialog.size());
        newUserDialog.exec();
    });

    connect(ui->existingUserButton, &QPushButton::clicked, this, [this] {
        QString existingUserDatabase = QFileDialog::getOpenFileName(
            this,
            "Open User Database",
            QString(), // Let the operating system decide which directory to default to
            "Impulse PHM Files (*.impulse)"
        );

        if (!existingUserDatabase.isEmpty()) {
            qDebug() << "Importing existing user database: " << existingUserDatabase;
        }
        try {
            impulse_core::db_import_user_database(existingUserDatabase.toStdString());
        }
        catch (const rust::Error& e) {
            util::notify::show_notification(
                this,
                QMessageBox::Critical,
                "Import Failed",
                "",
                QMessageBox::Ok,
                QMessageBox::Ok,
                "Failed to import the existing user database."
                );
        }
        util::notify::information(
            this,
            "Successful Import",
            "The database was imported successfully!"
        );
        // TODO: Use a utility that closes the welcome dialog and opens the main window
    });
}

void WelcomeDialog::setAllText() {
    this->ui->welcomeLabel->setText("Welcome to Impulse PHM!");
    this->ui->newUserButton->setText("&Create New User");
    this->ui->existingUserButton->setText("&Open Existing Database");
}

WelcomeDialog::~WelcomeDialog()
{
    delete ui;
}
