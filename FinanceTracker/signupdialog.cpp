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
    setFixedSize(320, 300);
    setStyleSheet(R"(
        QDialog {
            background-color: #fdfdfd;
            border-radius: 16px;
        }
        QLabel#titleLabel {
            font-size: 20px;
            font-weight: bold;
            margin-bottom: 12px;
        }
        QLineEdit {
            padding: 10px;
            border: 1px solid #ccc;
            border-radius: 8px;
            font-size: 14px;
        }
        QPushButton#signupButton {
            background-color: #3A5EFF;
            color: white;
            font-size: 14px;
            font-weight: bold;
            padding: 10px;
            border: none;
            border-radius: 8px;
        }
        QPushButton#signupButton:hover {
            background-color: #1d40e0;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(14);

    QLabel *titleLabel = new QLabel("회원가입");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("아이디를 입력하세요");

    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("비밀번호를 입력하세요");
    passwordEdit->setEchoMode(QLineEdit::Password);

    confirmEdit = new QLineEdit();
    confirmEdit->setPlaceholderText("비밀번호를 다시 입력하세요");
    confirmEdit->setEchoMode(QLineEdit::Password);

    QPushButton *signupButton = new QPushButton("회원가입");
    signupButton->setObjectName("signupButton");

    layout->addWidget(usernameEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(confirmEdit);
    layout->addWidget(signupButton);

    connect(signupButton, &QPushButton::clicked, this, &SignupDialog::handleSignup);
}

void SignupDialog::handleSignup() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();
    QString confirm = confirmEdit->text();

    if (username.isEmpty() || password.isEmpty() || confirm.isEmpty()) {
        QMessageBox::warning(this, "입력 오류", "모든 항목을 입력해주세요.");
        return;
    }

    if (password != confirm) {
        QMessageBox::warning(this, "비밀번호 불일치", "비밀번호가 일치하지 않습니다.");
        return;
    }

    if (UserDBManager::registerUser(username, password)) {
        QMessageBox::information(this, "가입 완료", "회원가입이 완료되었습니다.");
        accept();
    } else {
        QMessageBox::warning(this, "가입 실패", "이미 존재하는 아이디이거나 오류가 발생했습니다.");
    }
}
