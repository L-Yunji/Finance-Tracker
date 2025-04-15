#include "detailtransaction.h"
#include "TransactionData.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>

DetailTransaction::DetailTransaction(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(360, 640);
    setWindowTitle("상세 거래 내역");
    setupUI();
}

void DetailTransaction::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 32, 32, 32);
    mainLayout->setSpacing(20);
    this->setStyleSheet("background-color: white;");

    // 1. 카테고리
    categoryComboBox = new QComboBox(this);
    categoryComboBox->addItems({"월급", "용돈", "기타"});
    categoryComboBox->setStyleSheet("font-size: 18px; font-weight: bold;");
    mainLayout->addWidget(categoryComboBox);

    // 2. 메모 입력창
    memoEdit = new QLineEdit(this);
    memoEdit->setPlaceholderText("메모 입력");
    memoEdit->setStyleSheet("font-size: 14px;");
    mainLayout->addWidget(memoEdit);

    // 3. 구분선
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);

    // 4. 날짜 / 금액 / 계좌 잔액
    QVBoxLayout *infoLayout = new QVBoxLayout;
    infoLayout->setAlignment(Qt::AlignTop);

    dateLabel = new QLabel("", this);
    amountLabel = new QLabel("", this);
    balanceLabel = new QLabel("", this);

    QFont infoFont;
    infoFont.setPointSize(14);
    dateLabel->setFont(infoFont);
    amountLabel->setFont(infoFont);
    balanceLabel->setFont(infoFont);

    infoLayout->addWidget(dateLabel);
    infoLayout->addWidget(amountLabel);
    infoLayout->addWidget(balanceLabel);

    mainLayout->addLayout(infoLayout);

    // 5. 버튼
    updateBtn = new QPushButton("수정하기", this);
    updateBtn->setStyleSheet(R"(
    QPushButton {
        background-color: #B3D5FF;
        color: #1E40FF;
        font-weight: bold;
        font-size: 16px;
        border-radius: 10px;
    }
    QPushButton:hover {
        background-color: #1517D5;
        color: white;
    }
)");
    updateBtn->setFixedHeight(45);

    deleteBtn = new QPushButton("삭제하기", this);
    deleteBtn->setStyleSheet(R"(
    QPushButton {
        background-color: #D5D6DA;
        color: white;
        font-size: 16px;
        font-weight: bold;
        border-radius: 10px;
    }
    QPushButton:hover {
        background-color: #030303;
        color: white;
    }
)");
    deleteBtn->setFixedHeight(45);

    mainLayout->addWidget(updateBtn);
    mainLayout->addWidget(deleteBtn);
}

void DetailTransaction::setTransaction(const TransactionData &data)
{
    categoryComboBox->setCurrentText(data.category);
    memoEdit->setText(data.memo);

    dateLabel->setText("거래 일시: " + data.dateTime);
    amountLabel->setText("거래금액: " + data.amount + "원");
    balanceLabel->setText(data.isExpense ? "출금" : "입금");
    balanceLabel->setStyleSheet(data.isExpense ? "color: #1E40FF; font-weight: bold;" : "color: #E53935; font-weight: bold;");
}
