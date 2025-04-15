#include "maintransaction.h"
#include "TransactionData.h"
#include "userdbmanger.h"
#include "logindialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. 트랜잭션 데이터 타입 등록
    qRegisterMetaType<TransactionData>("TransactionData");

    UserDBManager::printAllUsers();
    // 2. SQLite DB 초기화
    if (!UserDBManager::initDB()) {
        return -1; // DB 초기화 실패 시 종료
    }

    // 3. 로그인 창 표시
    LoginDialog login;
    if (login.exec() != QDialog::Accepted) {
        return 0; // 로그인 취소 또는 실패 시 종료
    }

    // 4. 로그인 성공 시 메인 창 실행
    MainTransaction w;
    w.show();

    return a.exec();
}
