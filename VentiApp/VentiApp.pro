# 1. Qt 모듈 설정
QT += core gui sql widgets

# 2. 헤더 검색 경로 (INCLUDEPATH) 수정
INCLUDEPATH += $$PWD \
               $$PWD/common/src/header \
               $$PWD/../VentiCore \
               $$PWD/header # 이 경로가 정확히 포함되어 있어야 합니다.

# 3. 라이브러리 링크 (VentiCore)
LIBS += -L$$OUT_PWD/../VentiCore/ -lVentiCore

# 4. 소스 파일 목록 (cpp)
# ⚠️ 빌드 생성 파일(moc_*, qrc_*)은 절대 여기에 적지 마세요!
SOURCES += \
    main.cpp \
    beverage.cpp \
    cartwidget.cpp \
    categorywidget.cpp \
    main_page.cpp \
    mainwindow.cpp \
    common/src/connect.cpp \
    src/cart.cpp \
    src/cartmanager.cpp

# 5. 헤더 파일 목록 (h)
# ⚠️ 빌드 생성 파일(ui_*, moc_*)은 절대 여기에 적지 마세요!
HEADERS += \
    mainwindow.h \
    beverage.h \
    cartwidget.h \
    categorywidget.h \
    main_page.h \
    common/src/header/connect.h \
    header/cart.h \
    header/cartmanager.h \
    header/kioskevent.h

# 6. UI 파일 목록 (ui)
FORMS += mainwindow.ui
FORMS += beverage.ui
FORMS += cartwidget.ui
FORMS += categorywidget.ui
FORMS += main_page.ui

# 7. 리소스 파일 (qrc)
RESOURCES += venti_image.qrc
