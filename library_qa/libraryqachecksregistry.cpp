#include "libraryqachecksregistry.h"

#include "checks/checkpackagenamesandvalues.h"
#include "checks/checksymbolnamesandvalues.h"
#include "checks/devicesetpartprefixcheck.h"
#include "checks/icprefix.h"
#include "checks/symbolpingridcheck.h"

LibraryQAChecksRegistry *LibraryQAChecksRegistry::m_instance = nullptr;

LibraryQAChecksRegistry::LibraryQAChecksRegistry()
{
    m_checks << new CheckSymbolNamesAndValues();
    m_checks << new SymbolPinGridCheck();
    m_checks << new CheckPackageNamesAndValues();
    m_checks << new DevicesetPartPrefixCheck();
    m_checks << new ICPrefixCheck();

    for (LibraryQA_Check *check : m_checks) {
        if (check->target() == LibraryQA_Check::CheckTarget_Symbol)
            m_symbolChecks.append(check);
        else if (check->target() == LibraryQA_Check::CheckTarget_Package)
            m_packageChecks.append(check);
        else if (check->target() == LibraryQA_Check::CheckTarget_Deviceset)
            m_deviceSetChecks.append(check);
    }
}

LibraryQAChecksRegistry::~LibraryQAChecksRegistry()
{
    qDeleteAll(m_checks);
}

QList<LibraryQA_Check *> LibraryQAChecksRegistry::checks() const
{
    return m_checks;
}

LibraryQAChecksRegistry *LibraryQAChecksRegistry::instance()
{
    if (!m_instance)
        m_instance = new LibraryQAChecksRegistry();
    return m_instance;
}

QList<LibraryQA_Check *> LibraryQAChecksRegistry::symbolChecks() const
{
    return m_symbolChecks;
}

QList<LibraryQA_Check *> LibraryQAChecksRegistry::packageChecks() const
{
    return m_packageChecks;
}

QList<LibraryQA_Check *> LibraryQAChecksRegistry::deviceSetChecks() const
{
    return m_deviceSetChecks;
}
