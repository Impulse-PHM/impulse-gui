#include <QDate>

#include "newuserdialog.h"
#include "ui_newuserdialog.h"
#include "util/notification.h"

#include "rust/cxx.h"
#include "impulse-core/src/bridge.rs.h"


NewUserDialog::NewUserDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewUserDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Create New User");
    this->setAllText();
    this->setAllPlaceholderText();
    this->populateAll();
    this->addConnections();

}

void NewUserDialog::setAllText() {
    this->ui->createAccountLabel->setText("Create New User -- Impulse PHM");
    this->ui->firstNameLabel->setText("&First Name:");
    this->ui->lastNameLabel->setText("&Last Name:");
    this->ui->dateOfBirthLabel->setText("&Date of Birth:");
}

void NewUserDialog::setAllPlaceholderText() {
    this->ui->birthMonthComboBox->setPlaceholderText("Month");
    this->ui->birthDayComboBox->setPlaceholderText("Day");
    this->ui->birthYearComboBox->setPlaceholderText("Year");
}

void NewUserDialog::populateBirthMonth() {
    const std::vector<QString> months = {
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December"
    };

    for (int i = 0; i < static_cast<int>(months.size()); ++i) {
        this->ui->birthMonthComboBox->addItem(months[i], i + 1);
    }
}

void NewUserDialog::populateBirthDay() {
    for (int i = 0; i < 31; ++i) {
        int day = i + 1;
        this->ui->birthDayComboBox->addItem(QString::number(day), day);
    }
}

void NewUserDialog::populateBirthYear() {
    unsigned short miniumYear = 1900;
    unsigned short currentYear = QDate::currentDate().year();
    for (int y = currentYear; y >= miniumYear; --y) {
        ui->birthYearComboBox->addItem(QString::number(y), y);
    }
}

void NewUserDialog::populateAll() {
    this->populateBirthMonth();
    this->populateBirthDay();
    this->populateBirthYear();
}

void NewUserDialog::addConnections() {
    connect(this->ui->createAccountButtonBox, &QDialogButtonBox::accepted, this, [this] {
        QString informativeErrorMessage = "";

        QString first_name = this->ui->firstNameLineEdit->text().trimmed();
        if (first_name.isEmpty()) {
            informativeErrorMessage.append("Please enter your first name.");
            informativeErrorMessage.append("\n");
        }

        QString last_name = this->ui->lastNameLineEdit->text().trimmed();
        if (last_name.isEmpty()) {
            informativeErrorMessage.append("Please enter your last name.");
            informativeErrorMessage.append("\n");
        }

        int birthMonth = this->ui->birthMonthComboBox->currentData().toInt();
        if ( birthMonth == 0) {
            informativeErrorMessage.append("Please enter the month of your birth.");
            informativeErrorMessage.append("\n");
        }

        int birthDay = this->ui->birthDayComboBox->currentData().toInt();
        if (birthDay == 0) {
            informativeErrorMessage.append("Please enter the day of your birth.");
            informativeErrorMessage.append("\n");
        }

        int birthYear = this->ui->birthYearComboBox->currentData().toInt();
        if (birthYear == 0) {
            informativeErrorMessage.append("Please enter the year of your birth.");
            informativeErrorMessage.append("\n");
        }

        if (informativeErrorMessage.isEmpty()) {
            auto user = impulse_core::user_build_user(
                first_name.toStdString(),
                last_name.toStdString(),
                birthMonth,
                birthDay,
                birthYear
            );

            bool user_is_saved = false;
            try {
                impulse_core::user_save_user(user);

                // No exception thrown / successful save
                user_is_saved = true;
                util::notify::information(
                    this,
                    "Success",
                    "Your account was created successfully!"
                    );
            }
            catch (const rust::Error& e) {
                util::notify::show_notification(
                    this,
                    QMessageBox::Critical,
                    "Validation Failed",
                    "",
                    QMessageBox::Ok,
                    QMessageBox::Ok,
                    "Failed to save your information in the user database."
                );
            }

            if (user_is_saved) {
                this->accept();
            }

        }
        else {
            util::notify::show_notification(
                this,
                QMessageBox::Critical,
                "Validation Failed",
                "",
                QMessageBox::Ok,
                QMessageBox::Ok,
                informativeErrorMessage
            );
        }
    });

    connect(ui->createAccountButtonBox, &QDialogButtonBox::rejected, this, [this] {
        this->close();
    });
}

NewUserDialog::~NewUserDialog()
{
    delete ui;
}
