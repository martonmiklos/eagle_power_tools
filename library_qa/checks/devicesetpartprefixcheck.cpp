#include "devicesetpartprefixcheck.h"

DevicesetPartPrefixCheck::DevicesetPartPrefixCheck() :
    DeviceSetQACheck(QObject::tr("Verify that deviceset has part name prefix"))
{
    m_code = "deviceset_prefix_check";
}

bool DevicesetPartPrefixCheck::checkDeviceset(Deviceset *ds)
{
    return !ds->prefix().isEmpty();
}
