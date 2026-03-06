TEMPLATE = subdirs

# 빌드 순서 강제 옵션
CONFIG += ordered

# 반드시 라이브러리(Core)가 앱(App)보다 먼저 와야 합니다.
SUBDIRS += \
    VentiCore \
    VentiApp

# VentiApp이 VentiCore에 의존함을 명시적으로 설정 (권장)
VentiApp.depends = VentiCore
