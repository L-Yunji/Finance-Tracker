#include "addtransaction.h"
#include "TransactionData.h"
#include "TransactionStore.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QLocale>
#include <QComboBox>
#include <QMenu>

AddTransaction::AddTransaction(bool isExpense, const QString &username, QWidget *parent)
    : QWidget(parent),
    expenseFlag(isExpense),
    currentUsername(username),
    displayLabel(nullptr),
    keyboardWidget(nullptr),
    keyboardLayout(nullptr),
    Btn1(nullptr), Btn2(nullptr), Btn3(nullptr), Btn4(nullptr), Btn5(nullptr),
    Btn6(nullptr), Btn7(nullptr), Btn8(nullptr), Btn9(nullptr), Btn00(nullptr),
    Btn0(nullptr), BtnDelete(nullptr)
{
    setupUI();
    setupKeyboard();
    connect(backBtn, &QPushButton::clicked, this, &AddTransaction::close);
    setWindowTitle("내역 입력");
    setFixedSize(360, 640);
    setContentsMargins(32, 0, 32, 32);
}

AddTransaction::~AddTransaction() {}

void AddTransaction::setupUI()
{
    // 전체 배경 스타일
    this->setStyleSheet("background-color: white;");

    // 전체 메인 레이아웃
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    this->setLayout(mainLayout);

    // 1. 상단 헤더
    QHBoxLayout *headerLayout = new QHBoxLayout();
    QWidget *headerWidget = new QWidget();
    headerWidget->setFixedHeight(40);

    QGridLayout *innerHeaderLayout = new QGridLayout(headerWidget);
    innerHeaderLayout->setContentsMargins(0, 0, 0, 0);
    innerHeaderLayout->setColumnStretch(0, 1);
    innerHeaderLayout->setColumnStretch(1, 2);
    innerHeaderLayout->setColumnStretch(2, 1);

    backBtn = new QPushButton("뒤로");
    backBtn->setStyleSheet(R"(
    QPushButton {
        background-color: transparent;     /* 배경 투명 */
        border: none;
        border-radius: 16px;
        font-size: 12px;
        color: #4F4F4F;
    }
)");
    backBtn->setCursor(Qt::PointingHandCursor);  // 커서 모양 손바닥으로 변경

    QString titleText = expenseFlag ? "지출" : "수입";
    getSendHeader = new QLabel(titleText);
    getSendHeader->setAlignment(Qt::AlignCenter);
    QFont titleFont;
    titleFont.setPointSize(18);
    titleFont.setWeight(QFont::Black);
    getSendHeader->setFont(titleFont);
    getSendHeader->setStyleSheet("color: #4F4F4F;");

    innerHeaderLayout->addWidget(backBtn, 0, 0, Qt::AlignLeft);
    innerHeaderLayout->addWidget(getSendHeader, 0, 1, Qt::AlignCenter);
    headerWidget->setLayout(innerHeaderLayout);

    headerLayout->addWidget(headerWidget);
    mainLayout->addLayout(headerLayout);

    mainLayout->addSpacing(12);

    // 2. 금액 표시
    displayLabel = new QLabel("₩0");
    QFont font = displayLabel->font();
    font.setPointSize(30);
    font.setBold(true);
    displayLabel->setFont(font);
    displayLabel->setStyleSheet(R"(
    QLabel {
        color: #030303;
        qproperty-alignment: 'AlignRight';
    }
)");
    displayLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    displayLabel->setMinimumWidth(0);
    displayLabel->setMaximumWidth(QWIDGETSIZE_MAX);
    displayLabel->setFixedHeight(70);
    mainLayout->addWidget(displayLabel);
    mainLayout->addSpacing(12);

    // 3. 카테고리 버튼 + 메뉴로 교체
    categoryDropdownBtn = new QPushButton("카테고리 선택 ▾", this);
    categoryDropdownBtn->setCursor(Qt::PointingHandCursor);
    categoryDropdownBtn->setStyleSheet(R"(
    QPushButton {
        background-color: white;
        border: none;
        border-radius: 12px;
        padding: 8px 0px;
        font-size: 18px;
        color: #030303;
        text-align: left;
    }
    QPushButton::menu-indicator {
        width: 0px;
        height: 0px;
        image: none;
        subcontrol-position: right center;
        subcontrol-origin: content;
    }
)");

    categoryMenu = new QMenu(this);
    categoryMenu->setStyleSheet(R"(
    QMenu {
        background-color: white;
        border: none;
        border-radius: 12px;
        padding: 4px;
    }
    QMenu::item {
        background-color: white;
        color: #030303;
        font-size: 14px;
        padding: 10px 16px;
        border-radius: 8px;
        margin: 2px;
    }
    QMenu::item:selected {
        background-color: #B3D5FF;
        color: white;
    }
)");

    QStringList categoryList = expenseFlag
                                   ? QStringList{ "식비", "교통", "쇼핑", "기타" }
                                   : QStringList{ "월급", "용돈", "기타" };

    for (const QString &item : categoryList) {
        QAction *action = categoryMenu->addAction(item);
        connect(action, &QAction::triggered, this, [=]() {
            selectedCategory = item;
            categoryDropdownBtn->setText(item + " ▾");
        });
    }

    categoryDropdownBtn->setMenu(categoryMenu);
    mainLayout->addWidget(categoryDropdownBtn);
    mainLayout->addSpacing(8);


    // 4. Continue 버튼
    continueButton = new QPushButton("완료", this);
    continueButton->setStyleSheet(R"(
    QPushButton {
        background-color: #4F4F4F;
        color: white;
        font-size: 16px;
        font-weight: bold;
        padding: 12px 20px;
        border: none;
        border-radius: 12px;
    }
    QPushButton:hover {
        background-color: #303030;
    }
    QPushButton:pressed {
        background-color: #000000;
    }
)");
    continueButton->setFixedHeight(50);  // 높이 통일
    mainLayout->addWidget(continueButton);
    mainLayout->addSpacing(12);

    // continueBtnClick 시 slot 연결
    connect(continueButton, &QPushButton::clicked, this, &AddTransaction::handleContinueClicked);

}

void AddTransaction::handleKeyInput(const QString &input)
{
    QString currentText = displayLabel->text();
    currentText.remove(QRegularExpression("[^0-9]")); // 숫자만 남김

    if (currentText.length() >= 9) return;

    if (currentText == "0") {
        currentText = input;
    } else {
        currentText += input;
    }

    QLocale locale = QLocale::system();
    QString formatted = locale.toString(currentText.toLongLong());

    displayLabel->setText("₩" + formatted);
}

void AddTransaction::setupKeyboard()
{
    keyboardWidget = new QWidget();
    keyboardLayout = new QGridLayout(keyboardWidget);
    keyboardLayout->setSpacing(12);
    keyboardLayout->setContentsMargins(10, 10, 10, 10);

    // 숫자 키패드 라벨
    QStringList keys = { "1", "2", "3",
                        "4", "5", "6",
                        "7", "8", "9",
                        "00", "0", "←" };

    for (int i = 0; i < keys.size(); ++i) {
        int row = i / 3;
        int col = i % 3;

        QPushButton *btn = new QPushButton(keys[i]);
        btn->setFixedSize(70, 70);
        btn->setStyleSheet(R"(
            QPushButton {
                background-color: #F0F0F0;
                font-size: 20px;
                font-weight: bold;
                border: none;
                border-radius: 12px;
            }
            QPushButton:hover {
                background-color: #e0e0e0;
            }
        )");

        keyboardLayout->addWidget(btn, row, col);

        if (keys[i] == "←") {
            connect(btn, &QPushButton::clicked, this, &AddTransaction::deleteButtonClicked);
        } else {
            connect(btn, &QPushButton::clicked, [=]() {
                handleKeyInput(keys[i]);
            });
        }
    }

    keyboardWidget->setLayout(keyboardLayout);

    // 키패드를 메인 레이아웃에 추가
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout *>(this->layout());
    if (mainLayout) {
        mainLayout->addWidget(keyboardWidget);
    }
}

void AddTransaction::deleteButtonClicked()
{
    QString currentText = displayLabel->text();
    currentText.remove(QRegularExpression("[^0-9]"));  // 숫자만 남김

    if (currentText.length() <= 1) {
        currentText = "0";
    } else {
        currentText.chop(1);
    }

    QLocale locale = QLocale::system();
    QString formatted = locale.toString(currentText.toLongLong());
    displayLabel->setText("₩" + formatted);
}
void AddTransaction::handleContinueClicked()
{
    QString rawAmount = displayLabel->text();
    rawAmount.remove(QRegularExpression("[₩,\\s]"));
    long long inputAmount = rawAmount.toLongLong();

    // 1. 금액이 0원 이상인지 확인
    if (inputAmount == 0) {
        showWarningDialog("0원 이상 입력해주세요.");
        return;
    }

    // 2. 카테고리가 선택됐는지 확인
    if (selectedCategory.isEmpty()) {
        showWarningDialog("카테고리를 선택해주세요.");
        return;
    }

    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");

    // 3. 출금인데 잔액보다 많은 금액이면 차단
    long long currentBalance = 0;
    for (const TransactionData &data : TransactionStore::allTransactions) {
        long long amt = data.amount.toLongLong();
        currentBalance += data.isExpense ? -amt : amt;
    }

    if (expenseFlag && inputAmount > currentBalance) {
        showWarningDialog("현재 잔액보다 많은 금액을\n지출할 수 없습니다.");
        return;
    }

    // 4. 거래 데이터 저장
    TransactionData data;
    data.amount = QString::number(inputAmount);
    data.category = selectedCategory;
    data.dateTime = datetime;
    data.isExpense = expenseFlag;
    // data.memo = memoEdit->text(); // 필요 시 추가

    TransactionStore::addTransaction(currentUsername, data);

    emit transactionAdded();
    this->close();

    qDebug() << "[Transaction 저장됨]";
    qDebug() << data.amount << data.category << data.dateTime << data.isExpense;
}

void AddTransaction::showWarningDialog(const QString &message)
{
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("ERROR");
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

    QLabel *title = new QLabel("Warning!", dialog);
    title->setObjectName("Title");
    title->setAlignment(Qt::AlignCenter);

    QLabel *msg = new QLabel(message, dialog);
    msg->setObjectName("Message");
    msg->setAlignment(Qt::AlignCenter);

    QPushButton *okButton = new QPushButton("확인", dialog);
    okButton->setCursor(Qt::PointingHandCursor);
    okButton->setFixedWidth(100);
    connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(okButton);
    btnLayout->addStretch();

    layout->addWidget(title);
    layout->addWidget(msg);
    layout->addLayout(btnLayout);

    dialog->exec();
}

