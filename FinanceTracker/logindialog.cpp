#include "logindialog.h"
#include "userdbmanger.h"
#include "signupdialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QSpacerItem>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("로그인");
    setFixedSize(320, 360);
    setStyleSheet("background-color: white;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    // 🔹 제목 라벨
    QLabel *titleLabel = new QLabel("로그인");
    titleLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #2C2C2C;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    QFont inputFont("Pretendard", 13);

    // 🔹 아이디 입력창
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("아이디를 입력하세요");
    usernameEdit->setFixedHeight(44);
    usernameEdit->setFont(inputFont);
    usernameEdit->setStyleSheet(R"(
        QLineEdit {
            border: 1px solid #ccc;
            border-radius: 8px;
            padding: 0 12px;
            font-size: 14px;
        }
    )");
    mainLayout->addWidget(usernameEdit);

    // 🔹 비밀번호 입력창
    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("비밀번호를 입력하세요");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setFixedHeight(44);
    passwordEdit->setFont(inputFont);
    passwordEdit->setStyleSheet(usernameEdit->styleSheet());
    mainLayout->addWidget(passwordEdit);

    // 🔹 로그인 버튼
    QPushButton *loginButton = new QPushButton("로그인");
    loginButton->setFixedHeight(44);
    loginButton->setCursor(Qt::PointingHandCursor);
    loginButton->setStyleSheet(R"(
        QPushButton {
            background-color: #1E40FF;
            color: white;
            font-size: 16px;
            font-weight: bold;
            border: none;
            border-radius: 8px;
        }
        QPushButton:hover {
            background-color: #1735cc;
        }
    )");
    mainLayout->addWidget(loginButton);

    // 🔹 회원가입 링크 버튼
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();

    QLabel *noAccountLabel = new QLabel("아직 계정이 없으신가요?");
    noAccountLabel->setStyleSheet("font-size: 12px; color: #777777;");

    QPushButton *signupLink = new QPushButton("회원가입");
    signupLink->setStyleSheet(R"(
        QPushButton {
            background: transparent;
            color: #1E40FF;
            font-weight: bold;
            font-size: 12px;
            border: none;
            text-decoration: underline;
        }
        QPushButton:hover {
            color: #1517D5;
        }
    )");
    signupLink->setCursor(Qt::PointingHandCursor);

    bottomLayout->addWidget(noAccountLabel);
    bottomLayout->addWidget(signupLink);
    bottomLayout->addStretch();

    mainLayout->addSpacing(12);
    mainLayout->addLayout(bottomLayout);

    // 🔹 이벤트 연결
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::handleLogin);
    connect(signupLink, &QPushButton::clicked, this, &LoginDialog::openSignup);
}

void LoginDialog::handleLogin() {
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    if (UserDBManager::loginUser(username, password)) {
        emit loginSuccess(username);
        accept();
    } else {
        // 예쁜 커스텀 경고창
        QDialog *dialog = new QDialog(this);
        dialog->setFixedSize(300, 180);
        dialog->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

        dialog->setStyleSheet(R"(
            QDialog {
                background-color: #ffffff;
                border-radius: 16px;
                border: 1px solid #E0E0E0;
            }

            QLabel#Title {
                font-size: 18px;
                font-weight: bold;
                color: #2C2C2C;
            }

            QLabel#Message {
                font-size: 14px;
                color: #555555;
                padding: 0 20px;
                margin: 0px 10px;
            }

            QPushButton {
                background-color: #FF5E5E;
                color: white;
                font-size: 14px;
                font-weight: bold;
                padding: 8px 24px;
                border: none;
                border-radius: 6px;
            }

            QPushButton:hover {
                background-color: #E14C4C;
            }

            QPushButton:pressed {
                background-color: #B73838;
            }
        )");

        QVBoxLayout *layout = new QVBoxLayout(dialog);
        layout->setContentsMargins(20, 20, 20, 20);
        layout->setSpacing(12);

        QLabel *title = new QLabel("로그인 실패", dialog);
        title->setObjectName("Title");
        title->setAlignment(Qt::AlignCenter);

        QLabel *message = new QLabel("아이디 또는 비밀번호가 틀렸습니다.", dialog);
        message->setObjectName("Message");
        message->setAlignment(Qt::AlignCenter);
        message->setWordWrap(true);
        message->setContentsMargins(10, 0, 10, 0);

        QPushButton *okButton = new QPushButton("확인", dialog);
        okButton->setCursor(Qt::PointingHandCursor);
        okButton->setFixedWidth(100);
        connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);

        QHBoxLayout *btnLayout = new QHBoxLayout;
        btnLayout->addStretch();
        btnLayout->addWidget(okButton);
        btnLayout->addStretch();

        layout->addWidget(title);
        layout->addWidget(message);
        layout->addLayout(btnLayout);

        dialog->exec();
    }
}

void LoginDialog::openSignup() {
    SignupDialog signup;
    signup.exec();
}
