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
    main_page.cpp \
    mainwindow.cpp \
    option_modal.cpp

HEADERS += mainwindow.h \
    KioskData.h \
    beverage.h \
    cartwidget.h \
    categorywidget.h \
    main_page.h \
    option_modal.h

FORMS += mainwindow.ui \
    beverage.ui \
    cartwidget.ui \
    categorywidget.ui \
    main_page.ui \
    option_modal.ui

RESOURCES += \
    venti_image.qrc
