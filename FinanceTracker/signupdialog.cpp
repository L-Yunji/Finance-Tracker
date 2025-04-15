// signupdialog.cpp
#include "signupdialog.h"
#include "userdbmanger.h"

#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

SignupDialog::SignupDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("회원가입");
    setFixedSize(280, 200);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *titleLabel = new QLabel("회원가입");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    layout->addWidget(titleLabel);

    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("아이디");
    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("비밀번호");
    passwordEdit->setEchoMode(QLineEdit::Password);

    QPushButton *signupButton = new QPushButton("가입 완료");
    layout->addWidget(usernameEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(signupButton);

    connect(signupButton, &QPushButton::clicked, this, &SignupDialog::handleSignup);
}

void SignupDialog::handleSignup() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "입력 오류", "모든 필드를 입력해주세요.");
        return;
    }

    if (UserDBManager::registerUser(username, password)) {
        QMessageBox::information(this, "성공", "회원가입이 완료되었습니다.");
        accept();
    } else {
        QMessageBox::warning(this, "실패", "이미 존재하는 아이디이거나 오류가 발생했습니다.");
    }
}
