#-------------------------------------------------
#
# Project created by QtCreator 2014-01-07T00:12:41
#
#-------------------------------------------------

QT += core gui xml widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MagentoTools
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    configxml.cpp \
    rewrite.cpp \
    moduleconfig.cpp \
    mainwindow/rewrites.cpp \
    mainwindow/events.cpp \
    event.cpp \
    configxml/configxml2.cpp

HEADERS  += mainwindow.h \
    configxml.h \
    rewrite.h \
    moduleconfig.h \
    event.h

FORMS    += mainwindow.ui

RESOURCES += \
    magentotools.qrc

#QMAKE_LFLAGS = -static -static-libgcc -static-libstdc++

#CONFIG += release

#CONFIG += static
