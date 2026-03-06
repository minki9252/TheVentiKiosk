# TheVentiKiosk.pro (최상위 파일)
TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
    VentiCore \
    VentiApp

VentiApp.depends = VentiCore