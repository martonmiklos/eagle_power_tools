#include "kicadfootprintimport.h"

#include <QDebug>
#include <QFile>

KicadFootprintImport::KicadFootprintImport()
{

}

Library* KicadFootprintImport::parseFootprintFile(const QString &libraryPath)
{
    Library *lib = nullptr;
    QFile libFile(libraryPath);
    if (libFile.open(QFile::ReadOnly)) {
        lib = new Library();
        Packages *pkgs = new Packages();
        lib->setPackages(pkgs);
        Package *package = new Package();
        pkgs->addPackage(package);

        auto parsetree = sexpresso::parse(libFile.readAll().constData());
        auto module = parsetree.getChildByPath("module");

        package->setName(QString::fromStdString(module->getChild(1).toString()));

        auto description = module->getChildByPath("descr");
        Description *desc = new Description();
        desc->setValue(QString::fromStdString(description->getChild(1).toString()));
        package->setDescription(desc);

        size_t i = 0;
        while (1) {
            auto child = module->getChild(i);
            if (child.isNil())
                break;

            if (child.isSexp()) {
                auto key = child.getChild(0);
                if (key.isString()) {
                    QString keyStr = QString::fromStdString(key.toString());
                    if (keyStr == "fp_text") {
                        parseText(child, package);
                    } else if (keyStr == "fp_line") {
                        parseLine(child, package);
                    } else if (keyStr == "pad") {
                        parsePad(child, package);
                    }
                }
            }
            i++;
        }
        libFile.close();
    }
    return lib;
}

void KicadFootprintImport::parseText(sexpresso::Sexp &text, Package *package)
{
    QString textType = QString::fromStdString(text.getChild(1).toString());

    Text *eagleText = new Text();
    auto layer = text.getChildByPath("layer");
    int eagleLayer = -1;
    if (layer) {
        QString kicadLayer = QString::fromStdString(layer->getChild(1).toString());
        eagleLayer = KicadLayerToEAGLEMapper::mapKicadStringLayerToEagle(kicadLayer);
        eagleText->setLayer(eagleLayer);
    }

    auto at = text.getChildByPath("at");
    if (at) {
        eagleText->setX(QString::fromStdString(at->getChild(1).toString()).toDouble());
        eagleText->setY(-QString::fromStdString(at->getChild(2).toString()).toDouble());
    }

    auto font = text.getChildByPath("effects/font");
    if (font) {
        auto size = font->getChildByPath("size");
        eagleText->setSize(QString::fromStdString(size->getChild(2).toString()).toDouble());

        auto thickness = font->getChildByPath("thickness");
        int ratio = static_cast<int>(100.0 * (QString::fromStdString(thickness->getChild(1).toString()).toDouble() / eagleText->size()));
        eagleText->setRatio(ratio);
    }

    if (textType == "reference") {
        eagleText->setValue(">NAME");
    } else if (textType == "value") {
        eagleText->setLayer(QString::fromStdString(layer->getChild(1).toString()).startsWith("F")
                            ? 25
                            : 26);
        eagleText->setValue(">VALUE");
    } else {
        QString textValue = QString::fromStdString(text.getChild(2).toString());
        eagleText->setValue(textValue);
    }
    eagleText->setAlign(Text::Align_center);
    package->addText(eagleText);
}

void KicadFootprintImport::parsePad(sexpresso::Sexp &pad, Package *package)
{
    auto type = pad.getChild(2);
    if (type.isString()) {
        QString typeString = QString::fromStdString(type.toString());
        if (typeString == "smd") {
            parseSMD_Pad(pad, package);
        } else if (typeString == "thru_hole") {
            parseTH_Pad(pad, package);
        } else if (typeString == "np_thru_hole") {
            parseHole(pad, package);
        } else {
            qWarning() << "Unsupported pad type " << typeString;
        }
    }
}

void KicadFootprintImport::parseSMD_Pad(sexpresso::Sexp &smdPad, Package *package)
{
    Smd *smd = new Smd();
    auto at = smdPad.getChildByPath("at");
    if (at) {
        smd->setX(QString::fromStdString(at->getChild(1).toString()).toDouble());
        smd->setY(-QString::fromStdString(at->getChild(2).toString()).toDouble());
    }

    auto shape = smdPad.getChild(3);
    if (!shape.isNil()) {
        QString shapeStr = QString::fromStdString(shape.toString());
        if (shapeStr == "circle")
            smd->setRoundness(50);

    }

    auto size = smdPad.getChildByPath("size");
    if (size) {
        smd->setDx(QString::fromStdString(size->getChild(1).toString()).toDouble());
        smd->setDy(QString::fromStdString(size->getChild(2).toString()).toDouble());
    }

    auto layers = smdPad.getChildByPath("layers");
    smd->setStop(Smd::Stop_no);
    smd->setCream(Smd::Cream_no);
    for (size_t i = 1; i<layers->childCount(); i++) {
        if (layers->getChild(i).isString()) {
            QString kicadLayerName = QString::fromStdString(layers->getChild(i).toString());
            if (kicadLayerName.endsWith(".Cu")) {
                smd->setLayer(kicadLayerName.startsWith("F") ? 1 : 16);
            } else if (kicadLayerName.endsWith("Paste")) {
                smd->setCream(Smd::Cream_yes);
            } else if (kicadLayerName.endsWith("Mask")) {
                smd->setStop(Smd::Stop_yes);
            }
        }
    }

    smd->setName(QString::fromStdString(smdPad.getChild(1).toString()));
    package->addSmd(smd);
}

void KicadFootprintImport::parseTH_Pad(sexpresso::Sexp &thPad, Package *package)
{

}

void KicadFootprintImport::parseHole(sexpresso::Sexp &hole, Package *package)
{

}

void KicadFootprintImport::parseLine(sexpresso::Sexp &line, Package *package)
{
    Wire *wire = new Wire();
    auto layer = line.getChildByPath("layer");
    int eagleLayer = -1;
    if (layer) {
        QString kicadLayer = QString::fromStdString(layer->getChild(1).toString());
        eagleLayer = KicadLayerToEAGLEMapper::mapKicadStringLayerToEagle(kicadLayer);
        if (eagleLayer != -1) {
            wire->setLayer(eagleLayer);
        }
    }

    auto start = line.getChildByPath("start");
    if (start) {
        wire->setX1(QString::fromStdString(start->getChild(1).toString()).toDouble());
        wire->setY1(-QString::fromStdString(start->getChild(2).toString()).toDouble());
    }

    auto end = line.getChildByPath("end");
    if (end) {
        wire->setX2(QString::fromStdString(end->getChild(1).toString()).toDouble());
        wire->setY2(-QString::fromStdString(end->getChild(2).toString()).toDouble());
    }

    auto width = line.getChildByPath("width");
    if (width) {
        wire->setWidth(QString::fromStdString(width->getChild(1).toString()).toDouble());
    }

    package->addWire(wire);
}
