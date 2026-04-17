#include <QCoreApplication>
#include <QMessageBox>
#include <QTextFormat>

#include "notification.h"


namespace util::notify {
    void information(QWidget* parent, const QString& title, const QString& message) {
        showNotification(parent, QMessageBox::Information, title, message);
    }

    void warning(QWidget* parent, const QString& title, const QString& message) {
        showNotification(parent, QMessageBox::Warning, title, message);
    }

    void critical(QWidget* parent, const QString& title, const QString& message) {
        showNotification(parent, QMessageBox::Critical, title, message);
    }

    QMessageBox::StandardButton showNotification(
        QWidget* parent,
        QMessageBox::Icon icon,
        const QString& title,
        const QString& message,
        QMessageBox::StandardButtons buttons,
        QMessageBox::StandardButton defaultButton,
        const QString& informativeText,
        const QString& detailedText,
        Qt::TextFormat textFormat
        ) {
        QMessageBox messageBox(icon, title, message, buttons, parent);

        messageBox.setTextFormat(textFormat);
        messageBox.setDefaultButton(defaultButton);

        if (!informativeText.isEmpty()) {
            messageBox.setInformativeText(informativeText);
        }

        if (!detailedText.isEmpty()) {
            messageBox.setDetailedText(detailedText);
        }

        return static_cast<QMessageBox::StandardButton>(messageBox.exec());
    }
}
