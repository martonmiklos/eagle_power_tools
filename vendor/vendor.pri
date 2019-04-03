
DEFINES += QPM_INIT\\(E\\)=\"E.addImportPath(QStringLiteral(\\\"qrc:/\\\"));\"
DEFINES += QPM_USE_NS
INCLUDEPATH += $$PWD
QML_IMPORT_PATH += $$PWD


include($$PWD/de/skycoder42/dialog-master/dialogmaster.pri)
include($$PWD/de/skycoder42/qpathedit/qpathedit.pri)
