#include "checkpackagenamesandvalues.h"


CheckPackageNamesAndValues::CheckPackageNamesAndValues() :
    PackageQACheck(QObject::tr("Check name and value labels of the PCB footprints (packages)"))
{
    m_code = "check_package_name_and_values";
}

bool CheckPackageNamesAndValues::checkPackage(Package *pkg)
{
    bool hasValue = !m_checkValues;
    bool hasName = !m_checkNames;
    Text *nameText = nullptr;
    Text *valueText = nullptr;
    for (Text *text : *pkg->textList()) {
        // FIXME hardcoded names layer
        if (m_checkNames && text->value() == ">NAME") {
            if (text->layer() == 95)
                hasName = true;
            else
                nameText = text;
        }

        if (m_checkValues && text->value() == ">VALUE") {
            if (text->layer() == 96)
                hasValue = true;
            else
                valueText = text;
        }

        if ((hasName || nameText) && (hasValue || valueText))
            break;
    }

    if (m_checkNames && !hasName) {
        CheckPackageNamesAndValuesResult *res = new CheckPackageNamesAndValuesResult(pkg, CheckPackageNamesAndValuesResult::Name);
        res->setSeverity(LibraryQA_StepResult::Error);
        if (nameText) {
            res->setShortDescription(
                        QObject::tr(">NAME label is on wrong layer in the %1 package")
                        .arg(pkg->name()));
            res->setLongDescription(
                        QObject::tr("The >NAME label is on the %1 layer in the %2 package " \
                                    "however it should be on the Names (95) layer")
                        .arg(nameText->layer())
                        .arg(pkg->name()));
        } else {
            res->setShortDescription(
                        QObject::tr("Missing >NAME in the %1 package")
                        .arg(pkg->name()));
            res->setLongDescription(
                        QObject::tr("The %1 package does not have a >NAME text label on the Names layer")
                        .arg(pkg->name()));
        }
    }

    if (m_checkNames && !hasValue) {
        if (nameText) {
            CheckPackageNamesAndValuesResult *res = new CheckPackageNamesAndValuesResult(pkg,
                                                                                         CheckPackageNamesAndValuesResult::Value,
                                                                                         nameText);
            res->setShortDescription(
                        QObject::tr(">VALUE label is on wrong layer in the %1 package")
                        .arg(pkg->name()));
            res->setLongDescription(
                        QObject::tr("The >VALUE label is on the %1 layer in the %2 package " \
                                    "however it should be on the Values (96) layer")
                        .arg(nameText->layer())
                        .arg(pkg->name()));
            m_results.append(res);
        } else {
            CheckPackageNamesAndValuesResult *res = new CheckPackageNamesAndValuesResult(pkg,
                                                                                         CheckPackageNamesAndValuesResult::Value);
            res->setShortDescription(
                        QObject::tr("Missing >VALUE in the %1 package")
                        .arg(pkg->name()));
            res->setLongDescription(
                        QObject::tr("The %1 package does not have a >VALUE text label on the Values layer")
                        .arg(pkg->name()));
            m_results.append(res);
        }
    }

    return hasValue && hasName;
}


CheckPackageNamesAndValuesResult::CheckPackageNamesAndValuesResult(Package *pkg, Type type, Text *text) :
    m_pkg(pkg),
    m_type(type),
    m_text(text)
{

}

void CheckPackageNamesAndValuesResult::fix()
{
    if (!m_text)
        return;
    switch (m_type) {
    case Name:
        m_text->setLayer(25); // FIXME hardcoded tNames layer
        break;
    case Value:
        m_text->setLayer(27); // FIXME hardcoded tValues layer
        break;
    }
}

bool CheckPackageNamesAndValuesResult::fixable() const
{
    return m_text != nullptr;
}
