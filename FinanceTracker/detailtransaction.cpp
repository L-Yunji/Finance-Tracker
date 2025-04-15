#include "detailtransaction.h"
#include "TransactionData.h"
#include "TransactionStore.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QMessageBox>

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
    categoryComboBox->setStyleSheet(R"(
        QComboBox {
            font-size: 24px;
            font-weight: bold;
            border: none;
            background-color: transparent;
        }
    )");
    mainLayout->addWidget(categoryComboBox);

    // 2. 메모 입력창
    memoEdit = new QLineEdit(this);
    memoEdit->setPlaceholderText("메모 입력");
    memoEdit->setStyleSheet(R"(
        QLineEdit {
            font-size: 12px;
            border: none;
        }
    )");
    mainLayout->addWidget(memoEdit);

    // 3. 구분선
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);

    // 4. 정보 레이아웃 (거래 일시, 금액, 유형)
    QVBoxLayout *detailLayout = new QVBoxLayout;
    detailLayout->setAlignment(Qt::AlignTop);

    QFont labelFont;
    labelFont.setPointSize(12);
    QString labelStyle = "color: #4F4F4F; font-size: 12px;";

    QLabel *labelDateTitle = new QLabel("거래 일시:");
    labelDateTitle->setStyleSheet(labelStyle);
    labelDateTitle->setFont(labelFont);

    dateLabel = new QLabel("");
    dateLabel->setStyleSheet(labelStyle);
    dateLabel->setFont(labelFont);

    QHBoxLayout *dateLayout = new QHBoxLayout;
    dateLayout->addWidget(labelDateTitle);
    dateLayout->addStretch();
    dateLayout->addWidget(dateLabel);
    detailLayout->addLayout(dateLayout);

    QLabel *labelAmountTitle = new QLabel("거래 금액:");
    labelAmountTitle->setStyleSheet(labelStyle);
    labelAmountTitle->setFont(labelFont);

    amountLabel = new QLabel("");
    amountLabel->setStyleSheet(labelStyle);
    amountLabel->setFont(labelFont);

    QHBoxLayout *amountLayout = new QHBoxLayout;
    amountLayout->addWidget(labelAmountTitle);
    amountLayout->addStretch();
    amountLayout->addWidget(amountLabel);
    detailLayout->addLayout(amountLayout);

    QLabel *labelTypeTitle = new QLabel("거래 유형:");
    labelTypeTitle->setStyleSheet(labelStyle);
    labelTypeTitle->setFont(labelFont);

    balanceLabel = new QLabel("");
    balanceLabel->setTextFormat(Qt::RichText);
    balanceLabel->setFont(labelFont);

    QHBoxLayout *typeLayout = new QHBoxLayout;
    typeLayout->addWidget(labelTypeTitle);
    typeLayout->addStretch();
    typeLayout->addWidget(balanceLabel);
    detailLayout->addLayout(typeLayout);

    mainLayout->addLayout(detailLayout);

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
    connect(updateBtn, &QPushButton::clicked, this, &DetailTransaction::onUpdateClicked);

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
    connect(deleteBtn, &QPushButton::clicked, this, &DetailTransaction::onDeleteClicked);


    mainLayout->addWidget(updateBtn);
    mainLayout->addWidget(deleteBtn);
}

void DetailTransaction::setTransaction(const TransactionData &data)
{
    // 1. 출금/입금 여부에 따라 카테고리 항목 설정
    categoryComboBox->clear();
    if (data.isExpense) {
        categoryComboBox->addItems({"식비", "교통", "쇼핑", "기타"});
    } else {
        categoryComboBox->addItems({"월급", "용돈", "기타"});
    }
    categoryComboBox->setCurrentText(data.category);

    // 2. 메모
    memoEdit->setText(data.memo);

    // 3. 날짜 & 금액
    dateLabel->setText(data.dateTime);
    amountLabel->setText(data.amount + "원");

    // 4. 거래유형 텍스트
    QString typeText = data.isExpense ? "출금" : "입금";
    QString typeColor = data.isExpense ? "#1E40FF" : "#E53935";

    balanceLabel->setText(
        "<span style='color:" + typeColor + "; font-size:12px; font-weight: bold;'>"
        + typeText + "</span>");

    currentTransaction = data;
}

void DetailTransaction::onUpdateClicked()
{
    qDebug() << "수정 버튼 클릭!";
    for (TransactionData &item : TransactionStore::allTransactions) {
        if (item.dateTime == currentTransaction.dateTime && item.amount == currentTransaction.amount) {
            item.category = categoryComboBox->currentText();
            item.memo = memoEdit->text();
            emit transactionUpdated();
            this->close();
            return;
        }
    }
}

void DetailTransaction::onDeleteClicked()
{
    qDebug() << "삭제 버튼 클릭!";
    for (int i = 0; i < TransactionStore::allTransactions.size(); ++i) {
        const TransactionData &item = TransactionStore::allTransactions[i];
        if (item.dateTime == currentTransaction.dateTime && item.amount == currentTransaction.amount) {
            TransactionStore::allTransactions.removeAt(i);
            emit transactionDeleted();
            this->close();
            return;
        }
    }
}

