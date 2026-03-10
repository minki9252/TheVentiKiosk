QT += sql   # sq lite를 사용하기 위함

TARGET = VentiCore

TEMPLATE = lib  #.so 파일로 라이브러리화 하기 위함

DEFINES += VENTICORE_LIBRARY    # 라이브러리 지정

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += databasemanager.h \
           MenuDataLoader.h \
           venticore_global.h

SOURCES += databasemanager.cpp \
    MenuDataLoader.cpp

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

FORMS +=

