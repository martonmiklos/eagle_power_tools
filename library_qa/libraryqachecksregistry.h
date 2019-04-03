#ifndef LIBRARYQACHECKSREGISTRY_H
#define LIBRARYQACHECKSREGISTRY_H

#include "libraryqacheck.h"

#include <QList>

class LibraryQAChecksRegistry
{
public:
    LibraryQAChecksRegistry();
    ~LibraryQAChecksRegistry();

    QList<LibraryQA_Check *> checks() const;
    static LibraryQAChecksRegistry *instance();

    QList<LibraryQA_Check *> symbolChecks() const;
    QList<LibraryQA_Check *> packageChecks() const;
    QList<LibraryQA_Check *> deviceSetChecks() const;

private:
    QList<LibraryQA_Check*> m_checks;
    QList<LibraryQA_Check*> m_symbolChecks;
    QList<LibraryQA_Check*> m_packageChecks;
    QList<LibraryQA_Check*> m_deviceSetChecks;

    static LibraryQAChecksRegistry *m_instance;

};

#endif // LIBRARYQACHECKSREGISTRY_H
