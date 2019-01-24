#include "libraryqachecksregistry.h"

#include "checks/checknamesandvalues.h"

LibraryQAChecksRegistry::LibraryQAChecksRegistry()
{
    m_checks << new CheckNamesAndValues();
}
