// addtransaction.cpp
#include "addtransaction.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QString>
#include <QLocale>

AddTransaction::AddTransaction(QWidget *parent) :
    QWidget(parent),
    displayLabel(nullptr),
    keyboardWidget(nullptr),
    keyboardLayout(nullptr),
    Btn1(nullptr), Btn2(nullptr), Btn3(nullptr), Btn4(nullptr), Btn5(nullptr),
    Btn6(nullptr), Btn7(nullptr), Btn8(nullptr), Btn9(nullptr), Btn00(nullptr),
    Btn0(nullptr), BtnDelete(nullptr)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setupUI();
    setupKeyboard();
    mainLayout->addWidget(displayLabel);
    mainLayout->addWidget(keyboardWidget);
    setLayout(mainLayout);
    setWindowTitle("키패드 입력");
    setFixedSize(280, 500); // 크기 조절
}

AddTransaction::~AddTransaction()
{
}

void AddTransaction::setupUI()
{
    // 전체 배경 스타일
    this->setStyleSheet("background-color: #f0f2f5;");

    // 상단 헤더
    QHBoxLayout *headerLayout = new QHBoxLayout();
    QWidget *headerWidget = new QWidget();
    headerWidget->setStyleSheet("background-color: #3b82f6; border-top-left-radius: 20px; border-top-right-radius: 20px;");
    headerWidget->setFixedHeight(60);

    QHBoxLayout *innerHeaderLayout = new QHBoxLayout(headerWidget);

    backBtn = new QPushButton("←");
    backBtn->setFixedSize(40, 40);
    backBtn->setStyleSheet("color: white; font-size: 20px; background: transparent; border: none;");

    getSendHeader = new QLabel("보내기");
    getSendHeader->setAlignment(Qt::AlignCenter);
    QFont titleFont;
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    getSendHeader->setFont(titleFont);
    getSendHeader->setStyleSheet("color: white;");

    innerHeaderLayout->addWidget(backBtn);
    innerHeaderLayout->addStretch();
    innerHeaderLayout->addWidget(getSendHeader);
    innerHeaderLayout->addStretch();

    headerLayout->addWidget(headerWidget);

    // 금액 표시
    displayLabel = new QLabel("₩ 0");
    displayLabel->setAlignment(Qt::AlignCenter);
    QFont font = displayLabel->font();
    font.setPointSize(28);
    font.setBold(true);
    displayLabel->setFont(font);
    displayLabel->setStyleSheet("color: black; background-color: white; padding: 15px; border-radius: 15px;");
    displayLabel->setFixedHeight(70);

    // 카테고리 콤보박스
    categoryComboBox = new QComboBox(this);
    categoryComboBox->addItem("🍽️ 식비");
    categoryComboBox->addItem("🚌 교통");
    categoryComboBox->addItem("🛍️ 쇼핑");
    categoryComboBox->addItem("📦 기타");
    categoryComboBox->setStyleSheet("font-size: 14px; padding: 10px; border-radius: 10px; background-color: white;");
    categoryComboBox->setFixedHeight(45);

    // Continue 버튼
    continueButton = new QPushButton("Continue", this);
    continueButton->setStyleSheet("background-color: black; color: white; font-size: 16px; padding: 10px; border-radius: 10px;");
    continueButton->setFixedHeight(45);

    // 카테고리 + 버튼 레이아웃
    QVBoxLayout *categoryLayout = new QVBoxLayout();
    categoryLayout->addWidget(categoryComboBox);
    categoryLayout->addWidget(continueButton);

    // 전체 메인 레이아웃 구성
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout *>(this->layout());
    if (!mainLayout) {
        mainLayout = new QVBoxLayout(this);
        this->setLayout(mainLayout);
    }

    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->addLayout(headerLayout);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(displayLabel);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(categoryLayout);
}





void AddTransaction::setupKeyboard()
{
    keyboardWidget = new QWidget();
    keyboardLayout = new QGridLayout(keyboardWidget);
    keyboardLayout->setSpacing(10);
    keyboardLayout->setContentsMargins(10, 10, 10, 10);

    QStringList buttonLabels = { "1", "2", "3",
                                "4", "5", "6",
                                "7", "8", "9",
                                "00", "0", "←" };

    QList<QPushButton**> buttonPointers = {
        &Btn1, &Btn2, &Btn3,
        &Btn4, &Btn5, &Btn6,
        &Btn7, &Btn8, &Btn9,
        &Btn00, &Btn0, &BtnDelete
    };

    for (int i = 0; i < buttonLabels.size(); ++i) {
        int row = i / 3;
        int col = i % 3;
        QPushButton* btn = new QPushButton(buttonLabels[i]);
        btn->setFixedSize(50, 50);
        btn->setStyleSheet("font-size: 20px;");
        keyboardLayout->addWidget(btn, row, col);
        *buttonPointers[i] = btn;
    }

    keyboardWidget->setLayout(keyboardLayout);

    // 시그널 연결
    connect(Btn1, &QPushButton::clicked, this, &AddTransaction::buttonClicked);
    connect(Btn2, &QPushButton::clicked, this, &AddTransaction::buttonClicked);
    connect(Btn3, &QPushButton::clicked, this, &AddTransaction::buttonClicked);
    connect(Btn4, &QPushButton::clicked, this, &AddTransaction::buttonClicked);
    connect(Btn5, &QPushButton::clicked, this, &AddTransaction::buttonClicked);
    connect(Btn6, &QPushButton::clicked, this, &AddTransaction::buttonClicked);
    connect(Btn7, &QPushButton::clicked, this, &AddTransaction::buttonClicked);
    connect(Btn8, &QPushButton::clicked, this, &AddTransaction::buttonClicked);
    connect(Btn9, &QPushButton::clicked, this, &AddTransaction::buttonClicked);
    connect(Btn00, &QPushButton::clicked, this, &AddTransaction::buttonClicked);
    connect(Btn0, &QPushButton::clicked, this, &AddTransaction::buttonClicked);
    connect(BtnDelete, &QPushButton::clicked, this, &AddTransaction::deleteButtonClicked);
}

void AddTransaction::buttonClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (!clickedButton) return;

    QString currentText = displayLabel->text();
    QString buttonText = clickedButton->text();

    // 쉼표 제거
    currentText.remove(',');

    // 입력 초기화
    if (currentText == "0" && buttonText != ".") {
        currentText = buttonText;
    } else if (buttonText == ".") {
        if (!currentText.contains('.')) {
            currentText += buttonText;
        }
    } else {
        currentText += buttonText;
    }

    // 소수점 분리
    QString integerPart = currentText;
    QString decimalPart;

    if (currentText.contains('.')) {
        QStringList parts = currentText.split('.');
        integerPart = parts[0];
        decimalPart = parts[1];
    }

    // 숫자 포맷 적용 (정수 부분만)
    QLocale locale = QLocale::system(); // 또는 QLocale(QLocale::English) 등으로 명시 가능
    QString formattedText = locale.toString(integerPart.toLongLong());

    if (!decimalPart.isEmpty()) {
        formattedText += "." + decimalPart;
    }

    displayLabel->setText(formattedText);
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
