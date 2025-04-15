// logindialog.cpp
#include "logindialog.h"
#include "userdbmanger.h"
#include "signupdialog.h"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("로그인");
    setFixedSize(280, 200);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *titleLabel = new QLabel("로그인");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    layout->addWidget(titleLabel);

    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("아이디");
    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("비밀번호");
    passwordEdit->setEchoMode(QLineEdit::Password);

    QPushButton *loginButton = new QPushButton("로그인");
    QPushButton *signupButton = new QPushButton("회원가입");

    layout->addWidget(usernameEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(loginButton);
    layout->addWidget(signupButton);

    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::handleLogin);
    connect(signupButton, &QPushButton::clicked, this, &LoginDialog::openSignup);
}

void LoginDialog::handleLogin() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (UserDBManager::loginUser(username, password)) {
        emit loginSuccess(username);
        accept();
    } else {
        QMessageBox::warning(this, "실패", "아이디 또는 비밀번호가 틀렸습니다.");
    }
}

void LoginDialog::openSignup() {
    SignupDialog signup;
    signup.exec();
}
