cmake_minimum_required(VERSION 3.19)
project(FinanceTracker LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Sql)  # ✅ Sql 추가

qt_standard_project_setup()

qt_add_executable(FinanceTracker
    WIN32 MACOSX_BUNDLE
    main.cpp
    widget.cpp
    widget.h
    widget.ui
    addtransaction.h addtransaction.cpp
    maintransaction.h maintransaction.cpp
    icons.qrc
    TransactionData.h
    TransactionStore.h
    detailtransaction.h detailtransaction.cpp
    userdbmanger.h userdbmanger.cpp
    logindialog.h logindialog.cpp
)

target_link_libraries(FinanceTracker
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
)

# ... 이하 생략
