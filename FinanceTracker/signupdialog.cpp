#include "signupdialog.h"
#include "userdbmanger.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDialog>

SignupDialog::SignupDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("회원가입");
    setFixedSize(320, 360);
    setStyleSheet("background-color: white;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(16);

    QLabel *title = new QLabel("회원가입", this);
    title->setStyleSheet("font-size: 20px; font-weight: bold;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("아이디를 입력하세요");
    usernameEdit->setFixedHeight(40);
    usernameEdit->setFont(QFont("Pretendard", 12));
    usernameEdit->setStyleSheet("border: 1px solid #ccc; border-radius: 8px; padding-left: 10px;");
    layout->addWidget(usernameEdit);

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("비밀번호를 입력하세요");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setFixedHeight(40);
    passwordEdit->setFont(QFont("Pretendard", 12));
    passwordEdit->setStyleSheet("border: 1px solid #ccc; border-radius: 8px; padding-left: 10px;");
    layout->addWidget(passwordEdit);

    confirmEdit = new QLineEdit(this);
    confirmEdit->setPlaceholderText("비밀번호를 다시 입력하세요");
    confirmEdit->setEchoMode(QLineEdit::Password);
    confirmEdit->setFixedHeight(40);
    confirmEdit->setFont(QFont("Pretendard", 12));
    confirmEdit->setStyleSheet("border: 1px solid #ccc; border-radius: 8px; padding-left: 10px;");
    layout->addWidget(confirmEdit);

    QPushButton *signupBtn = new QPushButton("가입하기", this);
    signupBtn->setStyleSheet(R"(
        QPushButton {
            background-color: #005CFF;
            color: white;
            font-size: 16px;
            font-weight: bold;
            padding: 12px;
            border: none;
            border-radius: 8px;
        }
        QPushButton:hover {
            background-color: #0044cc;
        }
    )");
    signupBtn->setCursor(Qt::PointingHandCursor);
    layout->addWidget(signupBtn);

    connect(signupBtn, &QPushButton::clicked, this, &SignupDialog::handleSignup);
}

void SignupDialog::handleSignup() {
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text().trimmed();
    QString confirm = confirmEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty() || confirm.isEmpty()) {
        showWarningDialog("모든 항목을 입력해주세요.");
        return;
    }

    if (password != confirm) {
        showWarningDialog("비밀번호가 일치하지 않습니다.");
        return;
    }

    if (UserDBManager::registerUser(username, password)) {
        accept();  // 성공 시 종료
    } else {
        showWarningDialog("이미 존재하는 아이디이거나 오류가 발생했습니다.");
    }
}

void SignupDialog::showWarningDialog(const QString &message) {
    QDialog *dialog = new QDialog(this);
    dialog->setFixedSize(300, 180);
    dialog->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    dialog->setStyleSheet(R"(
        QDialog {
            background-color: #ffffff;
            border: 1px solid #E0E0E0;
            border-radius: 16px;
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

    QLabel *title = new QLabel("가입 실패", dialog);
    title->setObjectName("Title");
    title->setAlignment(Qt::AlignCenter);

    QLabel *messageLabel = new QLabel(message, dialog);
    messageLabel->setObjectName("Message");
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setWordWrap(true);

    QPushButton *okButton = new QPushButton("확인", dialog);
    okButton->setCursor(Qt::PointingHandCursor);
    okButton->setFixedWidth(100);
    connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(okButton);
    btnLayout->addStretch();

    layout->addWidget(title);
    layout->addWidget(messageLabel);
    layout->addLayout(btnLayout);

    dialog->exec();
}
