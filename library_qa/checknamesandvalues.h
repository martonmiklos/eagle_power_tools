#ifndef CHECKNAMESANDVALUES_H
#define CHECKNAMESANDVALUES_H

#include "libraryqa_step.h"

#include "qt_eagle_xml_parser/eagle.h"

class CheckNamesAndValues : public LibraryQA_Step
{
public:
    Severity check(const Library & library);

private:
    bool m_checkNames, m_checkValues;
    bool m_checkSymbols, m_checkPackages;
};

#endif // CHECKNAMESANDVALUES_H
