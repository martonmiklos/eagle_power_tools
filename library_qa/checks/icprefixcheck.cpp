#include "icprefix.h"

ICPrefixCheckResult::ICPrefixCheckResult(const QString& desctiption):
    LibraryQA_StepResult()
{
    setDescription(desctiption);
}

ICPrefixCheck::ICPrefixCheck()
{

}

bool ICPrefixCheck::checkDeviceset(Deviceset *ds)
{
    QMap<QString, QString> crossPrefixMap;
    crossPrefixMap["U"] = QStringLiteral("IC");
    crossPrefixMap["IC"] = QStringLiteral("U");

    QMapIterator<QString, QString> i(crossPrefixMap);
    QString errorString;
    while (i.hasNext()) {
        i.next();
        if (ds->name().startsWith(i.key())) {
            for (Deviceset *past : m_pastDevicesets) {
                if (past->name().startsWith(i.value())) {
                    if (!errorString.isEmpty())
                        errorString.append(", ");
                    errorString.append(QString(" the %1 deviceset has refdes prefix %2")
                                       .arg(past->name())
                                       .arg(i.value()));
                }
            }
        }
    }

    m_pastDevicesets << ds;
    return errorString.isEmpty();
}
