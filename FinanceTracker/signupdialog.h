// signupdialog.h
#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class SignupDialog : public QDialog {
    Q_OBJECT
public:
    explicit SignupDialog(QWidget *parent = nullptr);

private slots:
    void handleSignup();

private:
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
};

#endif // SIGNUPDIALOG_H
