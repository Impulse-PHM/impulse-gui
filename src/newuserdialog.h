#ifndef NEWUSERDIALOG_H
#define NEWUSERDIALOG_H

#include <QDialog>

namespace Ui {
class NewUserDialog;
}

class NewUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewUserDialog(QWidget *parent = nullptr);
    ~NewUserDialog();

private:
    Ui::NewUserDialog *ui;
    void setAllText();
    void setAllPlaceholderText();
    void populateBirthMonth();
    void populateBirthDay();
    void populateBirthYear();
    void populateAll();
    void addConnections();
};

#endif // NEWUSERDIALOG_H
