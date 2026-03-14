QT       += core gui sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


TARGET = VentiApp

TEMPLATE = app

SOURCES += \
           couponinputdialog.cpp \
           discountdialog.cpp \
           goodbyedialog.cpp \
           loadingdialog.cpp \
           main.cpp \
           mainwindow.cpp \
           menuoptiondialog.cpp \
           paymentdialog.cpp \
           phonestampdialog.cpp \
           receiptdialog.cpp

HEADERS += mainwindow.h \
           couponinputdialog.h \
           discountdialog.h \
           goodbyedialog.h \
           loadingdialog.h \
           menuoptiondialog.h \
           paymentdialog.h \
           phonestampdialog.h \
           receiptdialog.h

FORMS   += mainwindow.ui \
           couponinputdialog.ui \
           discountdialog.ui \
           menuoptiondialog.ui \
           paymentdialog.ui \
           phonestampdialog.ui

RESOURCES += venti_image.qrc


INCLUDEPATH += ../VentiCore
LIBS += -L$$OUT_PWD/../VentiCore -lVentiCore

# 기본 빌드 설정
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
