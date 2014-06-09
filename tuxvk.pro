TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    manager.cpp \
    player.cpp \
    usermodel.cpp \
    connector.cpp \
    longpoll.cpp

CONFIG += c++11
CONFIG += link_pkgconfig
PKGCONFIG += gtkmm-3.0 \
            libcurl \
            jsoncpp \
            webkitgtk-3.0


HEADERS += \
    manager.h \
    player.h \
    usermodel.h \
    connector.h \
    longpoll.h \
    structs.h

OTHER_FILES += \
    access.txt


