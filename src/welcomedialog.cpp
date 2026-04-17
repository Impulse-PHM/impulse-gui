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

        if (newUserDialog.exec() == QDialog::Accepted) {
            this->accept();
            emit mainWindowRequested();
        }
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

        bool databaseIsImported = false;
        try {
            impulse_core::resource_import_user_database(existingUserDatabase.toStdString());

            databaseIsImported = true;
            util::notify::information(
                this,
                "Successful Import",
                "The database was imported successfully!"
                );
        }
        catch (const rust::Error& e) {
            util::notify::showNotification(
                this,
                QMessageBox::Critical,
                "Import Failed",
                "",
                QMessageBox::Ok,
                QMessageBox::Ok,
                "Failed to import the existing user database."
                );
        }

        if (databaseIsImported) {
            this->accept();
            emit mainWindowRequested();
        }
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
