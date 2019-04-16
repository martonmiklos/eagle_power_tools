#ifndef ICPREFIX_H
#define ICPREFIX_H

#include "library_qa/libraryqacheck.h"

class ICPrefixCheckResult : public LibraryQA_StepResult
{
public:
    ICPrefixCheckResult(const QString& description);
};

class ICPrefixCheck : public DeviceSetQACheck
{
public:
    ICPrefixCheck();
    bool checkDeviceset(Deviceset *ds);
private:
    QList<Deviceset*> m_pastDevicesets;
};



#endif // ICPREFIX_H
