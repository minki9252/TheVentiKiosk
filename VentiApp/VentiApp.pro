QT       += core gui sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17


TARGET = VentiApp

TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp \
           categorywidget.cpp \
           cartwidget.cpp \
           main_page.cpp \
           beverage.cpp \
           menuoptiondialog.cpp

HEADERS += mainwindow.h \
           categorywidget.h \
           cartwidget.h \
           beverage.h \
           main_page.h \
           menuoptiondialog.h

FORMS   += mainwindow.ui \
           categorywidget.ui \
           cartwidget.ui \
           beverage.ui \
           main_page.ui \
           menuoptiondialog.ui

RESOURCES += venti_image.qrc


INCLUDEPATH += ../VentiCore
LIBS += -L$$OUT_PWD/../VentiCore -lVentiCore

# 기본 빌드 설정
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
