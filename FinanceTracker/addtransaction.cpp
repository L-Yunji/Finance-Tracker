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
    titleFont.setPointSize(14);
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
    font.setPointSize(32);
    font.setBold(true);
    displayLabel->setFont(font);
    displayLabel->setStyleSheet("color: #030303");
    displayLabel->setFixedHeight(70);
    mainLayout->addWidget(displayLabel);
    mainLayout->addSpacing(12);

    // 3. 카테고리 콤보박스
    categoryComboBox = new QComboBox(this);
    if (expenseFlag) {
        // 출금일 때
        categoryComboBox->addItem("식비");
        categoryComboBox->addItem("교통");
        categoryComboBox->addItem("쇼핑");
        categoryComboBox->addItem("기타");
    } else {
        // 입금일 때
        categoryComboBox->addItem("월급");
        categoryComboBox->addItem("용돈");
        categoryComboBox->addItem("기타");
    }
    categoryComboBox->setStyleSheet(R"(
    QComboBox {
        background-color: white;
        border: 1px solid #D5D6DA;
        border-radius: 12px;
        padding: 8px 12px;
        font-size: 14px;
        color: #333333;
    }

    QComboBox::drop-down {
        subcontrol-origin: padding;
        subcontrol-position: top right;
        width: 24px;
        border-left: 1px solid #D5D6DA;
    }

    QComboBox::down-arrow {
        width: 12px;
        height: 12px;
    }

    QComboBox QAbstractItemView {
        background-color: white;
        border: 1px solid #D5D6DA;
        selection-background-color: #B3D5FF;
        padding: 6px;
    }
)");
    categoryComboBox->setFixedHeight(45);  // 높이 통일
    mainLayout->addWidget(categoryComboBox);
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
    currentText.remove(',');

    if (currentText.length() <= 1) {
        displayLabel->setText("0");
        return;
    }

    currentText.chop(1);

    QString integerPart = currentText;
    QString decimalPart;

    if (currentText.contains('.')) {
        QStringList parts = currentText.split('.');
        integerPart = parts[0];
        decimalPart = parts[1];
    }

    QLocale locale = QLocale::system();
    QString formattedText = locale.toString(integerPart.toLongLong());

    if (!decimalPart.isEmpty()) {
        formattedText += "." + decimalPart;
    }

    displayLabel->setText(formattedText);
}

void AddTransaction::handleContinueClicked()
{
    QString rawAmount = displayLabel->text();
    rawAmount.remove(QRegularExpression("[₩,\\s]"));
    long long inputAmount = rawAmount.toLongLong();

    // 예외처리: 0원 이상만 등록 가능
    if (inputAmount == 0) {
        QDialog *dialog = new QDialog(this);
        dialog->setFixedSize(300, 180);
        dialog->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);  // 테두리 유지

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

        // 제목 라벨
        QLabel *title = new QLabel("Warning!", dialog);
        title->setObjectName("Title");
        title->setAlignment(Qt::AlignCenter);

        // 메시지 라벨
        QLabel *message = new QLabel("0원 이상 입력해주세요.", dialog);
        message->setObjectName("Message");
        message->setAlignment(Qt::AlignCenter);

        // 확인 버튼
        QPushButton *okButton = new QPushButton("확인", dialog);
        okButton->setCursor(Qt::PointingHandCursor);
        okButton->setFixedWidth(100);
        connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);

        // 버튼 중앙 배치
        QHBoxLayout *btnLayout = new QHBoxLayout;
        btnLayout->addStretch();
        btnLayout->addWidget(okButton);
        btnLayout->addStretch();

        // 배치 정리
        layout->addWidget(title);
        layout->addWidget(message);
        layout->addLayout(btnLayout);

        // 다이얼로그 실행
        dialog->exec();
        return;
    }

    QString category = categoryComboBox->currentText();
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");

    // 현재 총 잔액 계산
    long long currentBalance = 0;
    for (const TransactionData &data : TransactionStore::allTransactions) {
        long long amt = data.amount.toLongLong();
        currentBalance += data.isExpense ? -amt : amt;
    }

    // 예외처리: 출금인데 잔액보다 크면 차단
    if (expenseFlag && inputAmount > currentBalance) {
        QDialog *dialog = new QDialog(this);
        dialog->setFixedSize(300, 180);
        dialog->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);  // 테두리 O

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

        QLabel *message = new QLabel("현재 잔액보다 많은 금액을\n출금할 수 없습니다.", dialog);
        message->setObjectName("Message");
        message->setAlignment(Qt::AlignCenter);

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

        return; // 더 이상 진행하지 않음
    }

    TransactionData data;
    data.amount = QString::number(inputAmount);
    data.category = category;
    data.dateTime = datetime;
    data.isExpense = expenseFlag;

    TransactionStore::addTransaction(currentUsername, data);

    emit transactionAdded();
    this->close();

    qDebug() << "[Transaction 저장됨]";
    qDebug() << data.amount << data.category << data.dateTime << data.isExpense;
}
