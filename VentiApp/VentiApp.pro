# VentiApp.pro

QT += core gui widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# 라이브러리 헤더 위치 참조 (VentiCore 폴더 안의 헤더 사용)
INCLUDEPATH += $$PWD/../VentiCore

INCLUDEPATH += \
    $$PWD/Common \
    $$PWD/Coupon \
    $$PWD/Payment \
    $$PWD/Point \
    $$PWD/Receipt_Order

# 빌드된 .so 파일 링크 (상대 경로 주의)
# -L: 라이브러리 경로, -l: 라이브러리 이름 (lib와 .so 제외한 이름)
LIBS += -L$$OUT_PWD/../VentiCore/ -lVentiCore \
        -Wl,-rpath,$$OUT_PWD/../VentiCore

SOURCES += \
    Common/main.cpp \
    Common/beverage.cpp \
    Common/cartwidget.cpp \
    Common/categorywidget.cpp \
    Common/mainwindow.cpp \
    Common/option_modal.cpp \
    Coupon/coupondialog.cpp \
    Coupon/couponinputview.cpp \
    Coupon/couponmanagerwidget.cpp \
    Coupon/couponresultview.cpp \
    Coupon/couponselectview.cpp \
    Payment/paycompleteview.cpp \
    Payment/paymanagerwidget.cpp \
    Payment/paymentmaindialog.cpp \
    Payment/paymethodview.cpp \
    Payment/payprocessview.cpp \
    Point/pointinputview.cpp \
    Point/pointmanagerwidget.cpp \
    Point/pointresultview.cpp \
    Receipt_Order/ordernumberview.cpp \
    Receipt_Order/receiptmanagerwidget.cpp \
    Receipt_Order/receiptselectview.cpp

HEADERS += \
    Common/KioskData.h \
    Common/mainwindow.h \
    Common/beverage.h \
    Common/cartwidget.h \
    Common/categorywidget.h \
    Common/option_modal.h \
    Coupon/coupondialog.h \
    Coupon/couponinputview.h \
    Coupon/couponmanagerwidget.h \
    Coupon/couponresultview.h \
    Coupon/couponselectview.h \
    Payment/paycompleteview.h \
    Payment/paymanagerwidget.h \
    Payment/paymentdata.h \
    Payment/paymentmaindialog.h \
    Payment/paymethodview.h \
    Payment/payprocessview.h \
    Point/pointinputview.h \
    Point/pointmanagerwidget.h \
    Point/pointresultview.h \
    Receipt_Order/ordernumberview.h \
    Receipt_Order/receiptmanagerwidget.h \
    Receipt_Order/receiptselectview.h

FORMS += \
    Common/mainwindow.ui \
    Common/beverage.ui \
    Common/cartwidget.ui \
    Common/categorywidget.ui \
    Common/option_modal.ui \
    Coupon/couponinputview.ui \
    Coupon/couponmanagerwidget.ui \
    Coupon/couponresultview.ui \
    Coupon/couponselectview.ui \
    Payment/paycompleteview.ui \
    Payment/paymanagerwidget.ui \
    Payment/paymentmaindialog.ui \
    Payment/paymethodview.ui \
    Payment/payprocessview.ui \
    Point/pointinputview.ui \
    Point/pointmanagerwidget.ui \
    Point/pointresultview.ui \
    Receipt_Order/ordernumberview.ui \
    Receipt_Order/receiptmanagerwidget.ui \
    Receipt_Order/receiptselectview.ui

RESOURCES += \
    venti_image.qrc
