# 1. Qt 모듈 설정
QT += core gui sql widgets

# 2. 헤더 검색 경로 (INCLUDEPATH)
# VentiApp 최상위 폴더 (mainwindow.h 검색용)
INCLUDEPATH += $$PWD
# connect.h 가 있는 폴더 검색용
INCLUDEPATH += $$PWD/common/src/header
# VentiCore 라이브러리 헤더 검색용
INCLUDEPATH += $$PWD/../VentiCore

# 3. 라이브러리 링크 (VentiCore)
LIBS += -L$$OUT_PWD/../VentiCore/ -lVentiCore

# 4. 소스 파일 목록 (cpp)
# 🚨 주의: 새로 만든 폴더 안의 파일도 반드시 적어줘야 합니다!
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    common/src/connect.cpp

# 5. 헤더 파일 목록 (h)
HEADERS += \
    mainwindow.h \
    common/src/header/connect.h

# 6. UI 및 리소스 파일
FORMS += \
    mainwindow.ui

RESOURCES += \
    venti_image.qrc