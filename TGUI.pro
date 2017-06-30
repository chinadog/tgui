#-------------------------------------------------
#
# Проект TGUI создан 2017-01-29T12:41:15
#
#-------------------------------------------------

QT       += core gui sql

PROJECT = TGUI

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = $$PROJECT
CONFIG += link_pkgconfig
CONFIG += c++11

TEMPLATE = app

SOURCES += $$PWD/src/main.cpp\
           $$PWD/src/tgui.cpp \
    $$PWD/src/loglistwidget.cpp \
    src/menu.cpp \
    src/config.cpp \
    src/circleprogressbar.cpp \
    src/systemmonitor.cpp

HEADERS  += $$PWD/src/tgui.h \
    src/loglistwidget.h \
    src/menu.h \
    src/config.h \
    src/circleprogressbar.h \
    src/systemmonitor.h

DESTDIR = $$PWD/build/bin/
MOC_DIR = $$PWD/build/bin/
OBJECTS_DIR = $$PWD/build/bin/

isEmpty(PREFIX) {
    PREFIX=$$PWD
}
CONFIG_PATH = $$PREFIX/etc/$$PROJECT/
RESOURCES_PATH = $$PREFIX/share/$$PROJECT/

DEFINES += CONFIG_PATH=\\\"$$CONFIG_PATH\\\"
DEFINES += RESOURCES_PATH=\\\"$$RESOURCES_PATH\\\"

PKG_CONFIG_LIBDIR  = $$PREFIX/share/pkgconfig
PKG_CONFIG_LIBDIR += $$system(pkg-config --variable pc_path pkg-config)

PKGCONFIG += TLogger TDB Common

# installs
target.path     = $$PREFIX/bin

# config
config.path = $$PREFIX/etc/$$PROJECT
config.files += $$PWD/config/settings.conf

INSTALLS += target config
