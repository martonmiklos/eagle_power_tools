#ifndef DEVICESETPARTPREFIXCHECK_H
#define DEVICESETPARTPREFIXCHECK_H

#include "library_qa/libraryqacheck.h"

class DevicesetPartPrefixCheck : public DeviceSetQACheck
{
public:
    DevicesetPartPrefixCheck();
    bool checkDeviceset(Deviceset *ds) override;
};

#endif // DEVICESETPARTPREFIXCHECK_H
