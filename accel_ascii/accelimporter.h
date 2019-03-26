#ifndef ACCELIMPORTER_H
#define ACCELIMPORTER_H

#include "eagle.h"

#include "sexpresso/sexpresso/sexpresso.hpp"

class AccelImporter
{
public:
    AccelImporter();

    bool convert();

    void setSchs(const QStringList &schs);
    void setPcb(const QString &pcb);

private:
    Eagle *m_sch = nullptr;
    Eagle *m_pcb = nullptr;
    Eagle *m_lbr = nullptr;

    QString m_pcbPath;
    QStringList m_schPaths;
};

#endif // ACCELIMPORTER_H
