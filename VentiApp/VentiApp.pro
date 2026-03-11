# VentiApp.pro

QT += core gui widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# 라이브러리 헤더 위치 참조 (VentiCore 폴더 안의 헤더 사용)
INCLUDEPATH += $$PWD/../VentiCore

# 빌드된 .so 파일 링크 (상대 경로 주의)
# -L: 라이브러리 경로, -l: 라이브러리 이름 (lib와 .so 제외한 이름)
LIBS += -L$$OUT_PWD/../VentiCore/ -lVentiCore \
        -Wl,-rpath,$$OUT_PWD/../VentiCore

SOURCES += main.cpp \
    beverage.cpp \
    cartwidget.cpp \
    categorywidget.cpp \
    couponinputview.cpp \
    couponmanagerwidget.cpp \
    couponresultview.cpp \
    couponselectview.cpp \
    mainwindow.cpp \
    option_modal.cpp \
    ordernumberview.cpp \
    paycompleteview.cpp \
    paymanagerwidget.cpp \
    paymentmaindialog.cpp \
    paymethodview.cpp \
    payprocessview.cpp \
    pointinputview.cpp \
    pointmanagerwidget.cpp \
    pointresultview.cpp \
    receiptmanagerwidget.cpp \
    receiptselectview.cpp

HEADERS += mainwindow.h \
    KioskData.h \
    beverage.h \
    cartwidget.h \
    categorywidget.h \
    couponinputview.h \
    couponmanagerwidget.h \
    couponresultview.h \
    couponselectview.h \
    option_modal.h \
    ordernumberview.h \
    paycompleteview.h \
    paymanagerwidget.h \
    paymentdata.h \
    paymentmaindialog.h \
    paymethodview.h \
    payprocessview.h \
    pointinputview.h \
    pointmanagerwidget.h \
    pointresultview.h \
    receiptmanagerwidget.h \
    receiptselectview.h

FORMS += mainwindow.ui \
    beverage.ui \
    cartwidget.ui \
    categorywidget.ui \
    couponinputview.ui \
    couponmanagerwidget.ui \
    couponresultview.ui \
    couponselectview.ui \
    option_modal.ui \
    ordernumberview.ui \
    paycompleteview.ui \
    paymanagerwidget.ui \
    paymentmaindialog.ui \
    paymethodview.ui \
    payprocessview.ui \
    pointinputview.ui \
    pointmanagerwidget.ui \
    pointresultview.ui \
    receiptmanagerwidget.ui \
    receiptselectview.ui

RESOURCES += \
    venti_image.qrc
