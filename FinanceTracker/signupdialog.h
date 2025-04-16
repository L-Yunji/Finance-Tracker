#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QDialog>
#include <QLineEdit>

class SignupDialog : public QDialog {
    Q_OBJECT

public:
    explicit SignupDialog(QWidget *parent = nullptr);

private slots:
    void handleSignup();
    void showWarningDialog(const QString &message);
    void showSuccessDialog(const QString &message);

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *confirmEdit;
};

#endif // SIGNUPDIALOG_H
