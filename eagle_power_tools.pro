#-------------------------------------------------
#
# Project created by QtCreator 2018-11-27T21:51:51
#
#-------------------------------------------------

QT       += core gui

QT += widgets

TARGET = eagle_power_tools
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
RCC_DIR=build

DESTDIR = bin

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(qt_eagle_xml_parser/eagle.pri)
include(qmainwindow_with_recent_list/qmainwindow_with_recent_list.pri)

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    bommodel/bommodel.cpp \
    bommodel/bomitem.cpp \
    bommodel/bomattribute.cpp \
    eagledesignloader.cpp \
    silkscreenslicer.cpp \
    graphicsutilities.cpp \
    dialogsettings.cpp \
    eaglerc/palette.cpp \
    eaglerc/eaglesettings.cpp

HEADERS += \
        mainwindow.h \
    bommodel/bommodel.h \
    bommodel/bomitem.h \
    bommodel/bomattribute.h \
    eagledesignloader.h \
    silkscreenslicer.h \
    graphicsutilities.h \
    dialogsettings.h \
    eaglerc/palette.h \
    eaglerc/eaglesettings.h

FORMS += \
        mainwindow.ui \
    dialogsettings.ui
