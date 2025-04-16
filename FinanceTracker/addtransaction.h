// addtransaction.h
#ifndef ADDTRANSACTION_H
#define ADDTRANSACTION_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QDateTime>
#include <QMessageBox>

class AddTransaction : public QWidget
{
    Q_OBJECT

public:
    explicit AddTransaction(bool isExpense, const QString &username, QWidget *parent = nullptr);
    ~AddTransaction() override;

private:
    // UI elements
    QLabel *displayLabel;
    QWidget *keyboardWidget;
    QGridLayout *keyboardLayout;
    QPushButton *backBtn;
    QLabel *getSendHeader;
    QComboBox *categoryComboBox;
    QPushButton *continueButton;
    QString currentUsername;

private:
    QPushButton *categoryDropdownBtn;
    QMenu *categoryMenu;
    QString selectedCategory;


    // Keyboard buttons
    QPushButton *Btn1;
    QPushButton *Btn2;
    QPushButton *Btn3;
    QPushButton *Btn4;
    QPushButton *Btn5;
    QPushButton *Btn6;
    QPushButton *Btn7;
    QPushButton *Btn8;
    QPushButton *Btn9;
    QPushButton *Btn00;
    QPushButton *Btn0;
    QPushButton *BtnDelete;

private:
    bool expenseFlag;  // 출금이면 true, 입금이면 false

private slots:
    void deleteButtonClicked();
    void handleContinueClicked(); // 완료 버튼 눌렀을 때 호출 될 함수

signals:
    void transactionAdded();  // 거래 추가됨을 알리는 시그널

private:
    void setupUI();
    void setupKeyboard();
    void handleKeyInput(const QString &input);
    void buttonClicked();

};

#endif // ADDTRANSACTION_H
