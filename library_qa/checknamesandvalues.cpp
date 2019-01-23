#include "checknamesandvalues.h"


CheckNamesAndValues::~CheckNamesAndValues()
{
    qDeleteAll(m_results);
}

bool CheckNamesAndValues::check(Library *lib)
{
    bool ret = true;
    if (m_checkSymbols) {
        ret = checkSymbols(lib->symbols());
    }

    if (m_checkPackages) {
        ret &= checkPackage(lib->packages());
    }
    return ret;
}

bool CheckNamesAndValues::fix(Library *lib)
{
    for (LibraryQA_StepResult *res : m_results) {
        CheckNamesAndValuesResult *CNVRes = static_cast<CheckNamesAndValuesResult*>(res);
        switch (CNVRes->type()) {
        case CheckNamesAndValuesResult::Name:
            if (CNVRes->text()) {
                if (CNVRes->symbol())
                    CNVRes->text()->setLayer(95); // FIXME hardcoded Names layer
                else if (CNVRes->pkg())
                    CNVRes->text()->setLayer(25); // FIXME hardcoded tNames layer
            } else {
                //lib->pac
            }
            break;
        case CheckNamesAndValuesResult::Value:
            if (CNVRes->text()) {
                if (CNVRes->symbol())
                    CNVRes->text()->setLayer(96); // FIXME hardcoded Values layer
                else if (CNVRes->pkg())
                    CNVRes->text()->setLayer(27); // FIXME hardcoded tValues layer
            } else {
                //lib->pac
            }
            break;
        }
    }
    return false;
}

bool CheckNamesAndValues::checkPackage(Packages *packages)
{
    bool hasValue = !m_checkValues;
    bool hasName = !m_checkNames;
    for (Package *pkg : *packages->packageList()) {
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
            CheckNamesAndValuesResult *res = new CheckNamesAndValuesResult(CheckNamesAndValuesResult::Name);
            res->setPkg(pkg);
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
                res->setText(nameText);
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
            CheckNamesAndValuesResult *res = new CheckNamesAndValuesResult(CheckNamesAndValuesResult::Value);
            res->setPkg(pkg);
            res->setSeverity(LibraryQA_StepResult::Error);
            if (nameText) {
                res->setShortDescription(
                            QObject::tr(">VALUE label is on wrong layer in the %1 package")
                            .arg(pkg->name()));
                res->setLongDescription(
                            QObject::tr("The >VALUE label is on the %1 layer in the %2 package " \
                                        "however it should be on the Values (96) layer")
                            .arg(valueText->layer())
                            .arg(pkg->name()));
                res->setText(valueText);
            } else {
                res->setShortDescription(
                            QObject::tr("Missing >VALUE in the %1 package")
                            .arg(pkg->name()));
                res->setLongDescription(
                            QObject::tr("The %1 package does not have a >VALUE text label on the Values layer")
                            .arg(pkg->name()));
            }
        }
    }

    return hasValue && hasName;
}

bool CheckNamesAndValues::checkSymbols(Symbols *symbols)
{
    bool hasValue = !m_checkValues;
    bool hasName = !m_checkNames;
    for (Symbol *sym : *symbols->symbolList()) {
        Text *nameText = nullptr;
        Text *valueText = nullptr;
        for (Text *text : *sym->textList()) {
            // FIXME hardcoded tNames layer
            if (m_checkNames && text->value() == ">NAME") {
                if (text->layer() == 25)
                    hasName = true;
                else
                    nameText = text;
            }

            // FIXME hardcoded tValues layer
            if (m_checkValues && text->value() == ">VALUE") {
                if (text->layer() == 27)
                    hasValue = true;
                else
                    valueText = text;
            }

            if ((hasName || nameText) && (hasValue || valueText))
                break;
        }

        if (m_checkNames && !hasName) {
            CheckNamesAndValuesResult *res = new CheckNamesAndValuesResult(CheckNamesAndValuesResult::Name);
            res->setSymbol(sym);
            res->setSeverity(LibraryQA_StepResult::Error);
            if (nameText) {
                res->setShortDescription(
                            QObject::tr(">NAME label is on wrong layer in the %1 symbol")
                            .arg(sym->name()));
                res->setLongDescription(
                            QObject::tr("The >NAME label is on the %1 layer in the %2 symbol " \
                                        "however it should be on the tNames (25) layer")
                            .arg(nameText->layer())
                            .arg(sym->name()));
                res->setText(nameText);
            } else {
                res->setShortDescription(
                            QObject::tr("Missing >NAME in the %1 package")
                            .arg(sym->name()));
                res->setLongDescription(
                            QObject::tr("The %1 symbol does not have a >NAME text label on the tNames layer")
                            .arg(sym->name()));
            }
        }

        if (m_checkNames && !hasValue) {
            CheckNamesAndValuesResult *res = new CheckNamesAndValuesResult(CheckNamesAndValuesResult::Value);
            res->setSymbol(sym);
            res->setSeverity(LibraryQA_StepResult::Error);
            if (nameText) {
                res->setShortDescription(
                            QObject::tr(">VALUE label is on wrong layer in the %1 symbol")
                            .arg(sym->name()));
                res->setLongDescription(
                            QObject::tr("The >VALUE label is on the %1 layer in the %2 symbol " \
                                        "however it should be on the tValues (27) layer")
                            .arg(valueText->layer())
                            .arg(sym->name()));
                res->setText(valueText);
            } else {
                res->setShortDescription(
                            QObject::tr("Missing >VALUE in the %1 symbol")
                            .arg(sym->name()));
                res->setLongDescription(
                            QObject::tr("The %1 symbol does not have a >VALUE text label on the tValues layer")
                            .arg(sym->name()));
            }
        }
    }

    return hasValue && hasName;
}

CheckNamesAndValuesResult::CheckNamesAndValuesResult(Type type, Text *text) :
    m_type(type),
    m_text(text)
{

}

Package *CheckNamesAndValuesResult::pkg() const
{
    return m_pkg;
}

Text *CheckNamesAndValuesResult::text() const
{
    return m_text;
}

void CheckNamesAndValuesResult::setText(Text *text)
{
    m_text = text;
}

CheckNamesAndValuesResult::Type CheckNamesAndValuesResult::type() const
{
    return m_type;
}

Symbol *CheckNamesAndValuesResult::symbol() const
{
    return m_symbol;
}

void CheckNamesAndValuesResult::setPkg(Package *pkg)
{
    m_pkg = pkg;
}

void CheckNamesAndValuesResult::setSymbol(Symbol *symbol)
{
    m_symbol = symbol;
}
