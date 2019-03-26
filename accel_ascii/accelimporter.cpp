#include "accelimporter.h"

AccelImporter::AccelImporter()
{

}

bool AccelImporter::convert()
{
    bool ok = false;
    delete m_pcb;
    delete m_sch;
    delete m_lbr;

    m_pcb = new Eagle();
    m_sch = new Eagle();
    m_lbr = new Eagle();


    QFile pcbFile(m_pcbPath);
    if (pcbFile.open(QFile::ReadOnly)) {
        auto parsetree = sexpresso::parse(pcbFile.readAll().constData());
        auto module = parsetree.getChildByPath("module");

        package->setName(QString::fromStdString(module->getChild(1).toString()));

        auto description = module->getChildByPath("descr");
        Description *desc = new Description();
        desc->setValue(QString::fromStdString(description->getChild(1).toString()));
        package->setDescription(desc);

        for (size_t i = 0; i<module->childCount(); i++) {
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
        }
        pcbFile.close();
    }
    return lib;
    auto parsetree = sexpresso::parse(m_pcb.readAll().constData());
    auto module = parsetree.getChildByPath("module");
}

void AccelImporter::setSchs(const QStringList &schs)
{
    m_schPaths = schs;
}

void AccelImporter::setPcb(const QString &pcb)
{
    m_pcbPath = pcb;
}
