QT       += core gui sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


TARGET = VentiApp

TEMPLATE = app

SOURCES += \
           couponinputdialog.cpp \
           discountdialog.cpp \
           main.cpp \
           mainwindow.cpp \
           menuoptiondialog.cpp

HEADERS += mainwindow.h \
           couponinputdialog.h \
           discountdialog.h \
           menuoptiondialog.h

FORMS   += mainwindow.ui \
           couponinputdialog.ui \
           discountdialog.ui \
           menuoptiondialog.ui

RESOURCES += venti_image.qrc


INCLUDEPATH += ../VentiCore
LIBS += -L$$OUT_PWD/../VentiCore -lVentiCore

# 기본 빌드 설정
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
