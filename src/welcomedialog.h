#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>

namespace Ui {
class WelcomeDialog;
}

class WelcomeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeDialog(QWidget *parent = nullptr);
    ~WelcomeDialog();

signals:
    void mainWindowRequested();

private:
    Ui::WelcomeDialog *ui;
    void setAllText();
};

#endif // WELCOMEDIALOG_H
