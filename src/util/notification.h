/**
 * Utility functions to make showing notifications more convenient in Qt
 */

#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QMessageBox>
#include <QString>
#include <QWidget>

namespace util::notify {
    /// Create an "information" notification
    void information(QWidget* parent, const QString& title, const QString& message);

    /// Create a "warning" notification
    void warning(QWidget* parent, const QString& title, const QString& message);

    /// Create a "critical" notification
    void critical(QWidget* parent, const QString& title, const QString& message);

    /// Show a custom notification
    QMessageBox::StandardButton show_notification(
        QWidget* parent,
        QMessageBox::Icon icon,
        const QString& title,
        const QString& message,
        QMessageBox::StandardButtons buttons = QMessageBox::Ok,
        QMessageBox::StandardButton defaultButton = QMessageBox::Ok,
        const QString& informativeText = QString(),
        const QString& detailedText = QString(),
        Qt::TextFormat textFormat = Qt::PlainText
        );
}


#endif // NOTIFICATION_H
