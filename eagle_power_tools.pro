#-------------------------------------------------
#
# Project created by QtCreator 2018-11-27T21:51:51
#
#-------------------------------------------------

QT       += core gui widgets svg

TARGET = eagle_power_tools
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS SEXPRESSO_OPT_OUT_PIKESTYLE

CONFIG += c++11

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
include(vendor/vendor.pri)

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
    eaglerc/eaglesettings.cpp \
    dialoggraphicsdebug.cpp \
    gerberimportfixer.cpp \
    mpc/mpc.c \
    kicad/kicadlegacyfootprintimport.cpp \
    kicad/kicadimportutils.cpp \
    kicad/kicadlegacysymbolfileimport.cpp \
    kicad/kicadfootprintimport.cpp \
    sexpresso/sexpresso/sexpresso.cpp \
    eagle_librarysaver.cpp \
    library_qa/checks/symbolpingridcheck.cpp \
    library_qa/wizard/libraryqawizardselectlibrarypage.cpp \
    library_qa/wizard/libraryqawizard.cpp \
    library_qa/wizard/libraryqaselectchecks.cpp \
    library_qa/wizard/libraryqaselectcomponents.cpp \
    library_qa/wizard/libraryqaresults.cpp \
    library_qa/wizard/models/libraryqachecksmodel.cpp \
    library_qa/wizard/models/libraryelementsmodel.cpp \
    library_qa/libraryqacheck.cpp \
    library_qa/libraryqachecksregistry.cpp \
    accel_ascii/dialogaccelasciiimport.cpp \
    accel_ascii/dialogaccelasciiimport_settings.cpp \
    accel_ascii/accelimporter.cpp \
    library_qa/wizard/models/libraryqaresultsmodel.cpp \
    library_qa/checks/checksymbolnamesandvalues.cpp \
    library_qa/checks/checkpackagenamesandvalues.cpp \
    library_qa/checks/devicesetpartprefixcheck.cpp \
    lasercut/stencillasercutsvgexport.cpp \
    lasercut/lasercutstencilexportwizard.cpp \
    lasercut/wizardpages/lasercutintropage.cpp \
    lasercut/wizardpages/lasercutoutputpage.cpp \
    lasercut/wizardpages/lasercutinputpage.cpp \
    lasercut/wizardpages/lasercutfinishpage.cpp \
    library_qa/checks/icprefixcheck.cpp

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
    eaglerc/eaglesettings.h \
    dialoggraphicsdebug.h \
    gerberimportfixer.h \
    mpc/mpc.h \
    kicad/kicadlegacyfootprintimport.h \
    kicad/kicadimportutils.h \
    kicad/kicadlegacysymbolfileimport.h \
    kicad/kicadfootprintimport.h \
    sexpresso/sexpresso/sexpresso.hpp \
    eagle_librarysaver.h \
    library_qa/checks/symbolpingridcheck.h \
    library_qa/wizard/libraryqawizardselectlibrarypage.h \
    library_qa/wizard/libraryqawizard.h \
    library_qa/wizard/libraryqaselectchecks.h \
    library_qa/wizard/libraryqaselectcomponents.h \
    library_qa/wizard/libraryqaresults.h \
    library_qa/wizard/models/libraryqachecksmodel.h \
    library_qa/wizard/models/libraryelementssmodel.h \
    library_qa/libraryqacheck.h \
    library_qa/libraryqachecksregistry.h \
    accel_ascii/dialogaccelasciiimport.h \
    accel_ascii/accelimporter.h \
    kicad/kicadlegacysymbolgrammar.h \
    library_qa/wizard/models/libraryqaresultsmodel.h \
    library_qa/checks/checksymbolnamesandvalues.h \
    library_qa/checks/checkpackagenamesandvalues.h \
    library_qa/checks/devicesetpartprefixcheck.h \
    lasercut/stencillasercutsvgexport.h \
    lasercut/lasercutstencilexportwizard.h \
    lasercut/wizardpages/lasercutintropage.h \
    lasercut/wizardpages/lasercutoutputpage.h \
    lasercut/wizardpages/lasercutinputpage.h \
    lasercut/wizardpages/lasercutfinishpage.h \
    library_qa/checks/icprefix.h

FORMS += \
        mainwindow.ui \
    dialogsettings.ui \
    dialoggraphicsdebug.ui \
    library_qa/wizard/libraryqawizardselectlibrarypage.ui \
    library_qa/wizard/libraryqaselectchecks.ui \
    library_qa/wizard/libraryqaselectcomponents.ui \
    library_qa/wizard/libraryqaresults.ui \
    accel_ascii/dialogaccelasciiimport.ui \
    lasercut/wizardpages/lasercutintropage.ui \
    lasercut/wizardpages/lasercutoutputpage.ui \
    lasercut/wizardpages/lasercutinputpage.ui \
    lasercut/wizardpages/lasercutfinishpage.ui

RESOURCES += \
    res/resources.qrc
