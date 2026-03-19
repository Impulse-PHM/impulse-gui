#include "welcomedialog.h"
#include "ui_welcomedialog.h"
#include "newuserdialog.h"


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
