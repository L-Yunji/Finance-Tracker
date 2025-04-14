// addtransaction.h
#ifndef ADDTRANSACTION_H
#define ADDTRANSACTION_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QDateTime>

class AddTransaction : public QWidget
{
    Q_OBJECT

public:
    explicit AddTransaction(QWidget *parent = nullptr);
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
