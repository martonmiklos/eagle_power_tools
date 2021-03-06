#include "checksymbolnamesandvalues.h"

#include "qt_eagle_xml_parser/eaglelayers.h"

CheckSymbolNamesAndValues::CheckSymbolNamesAndValues() :
    SymbolQACheck (QObject::tr("Check name and value labels of the schematic symbols"))
{
    m_code = "check_symbol_name_and_values";
}

bool CheckSymbolNamesAndValues::checkSymbol(Symbol *sym)
{
    bool hasValue = !m_checkValues;
    bool hasName = !m_checkNames;
    Text *nameText = nullptr;
    Text *valueText = nullptr;
    for (Text *text : *sym->textList()) {
        if (m_checkNames && text->value() == ">NAME") {
            if (text->layer() == EagleLayers::tNames)
                hasName = true;
            else
                nameText = text;
        }

        if (m_checkValues && text->value() == ">VALUE") {
            if (text->layer() == EagleLayers::tValues)
                hasValue = true;
            else
                valueText = text;
        }

        if ((hasName || nameText) && (hasValue || valueText))
            break;
    }

    if (m_checkNames && !hasName) {
        if (nameText) {
            CheckSymbolNamesAndValuesResult *res = new CheckSymbolNamesAndValuesResult(
                        sym,
                        CheckSymbolNamesAndValuesResult::Name,
                        nameText);
            res->setShortDescription(
                        QObject::tr(">NAME label is on wrong layer in the %1 symbol")
                        .arg(sym->name()));
            res->setLongDescription(
                        QObject::tr("The >NAME label is on the %1 layer in the %2 symbol " \
                                    "however it should be on the tNames (25) layer")
                        .arg(nameText->layer())
                        .arg(sym->name()));
            m_results.append(res);
        } else {
            CheckSymbolNamesAndValuesResult *res = new CheckSymbolNamesAndValuesResult(
                        sym,
                        CheckSymbolNamesAndValuesResult::Name);
            res->setShortDescription(
                        QObject::tr("Missing >NAME in the %1 symbol")
                        .arg(sym->name()));
            res->setLongDescription(
                        QObject::tr("The %1 symbol does not have a >NAME text label on the tNames layer")
                        .arg(sym->name()));
            m_results.append(res);
        }
    }

    if (m_checkNames && !hasValue) {
        if (valueText) {
            CheckSymbolNamesAndValuesResult *res = new CheckSymbolNamesAndValuesResult(
                        sym,
                        CheckSymbolNamesAndValuesResult::Value,
                        valueText);
            res->setShortDescription(
                        QObject::tr(">VALUE label is on wrong layer in the %1 symbol")
                        .arg(sym->name()));
            res->setLongDescription(
                        QObject::tr("The >VALUE label is on the %1 layer in the %2 symbol " \
                                    "however it should be on the tValues (27) layer")
                        .arg(valueText->layer())
                        .arg(sym->name()));
            m_results.append(res);
        } else {
            CheckSymbolNamesAndValuesResult *res = new CheckSymbolNamesAndValuesResult(
                        sym,
                        CheckSymbolNamesAndValuesResult::Value);
            res->setShortDescription(
                        QObject::tr("Missing >VALUE in the %1 symbol")
                        .arg(sym->name()));
            res->setLongDescription(
                        QObject::tr("The %1 symbol does not have a >VALUE text label on the tValues layer")
                        .arg(sym->name()));
            m_results.append(res);
        }
    }

    return hasValue && hasName;
}

CheckSymbolNamesAndValuesResult::CheckSymbolNamesAndValuesResult(Symbol *symbol, Type type, Text *text) :
    m_symbol(symbol),
    m_type(type),
    m_text(text)
{
    setSeverity(Error);

    if (text) {
        setShortDescription(
                    QObject::tr(">NAME label is on wrong layer in the %1 symbol")
                    .arg(symbol->name()));
        setLongDescription(
                    QObject::tr("The >NAME label is on the %1 layer in the %2 symbol " \
                                "however it should be on the tNames (25) layer")
                    .arg(text->layer())
                    .arg(symbol->name()));
    } else {
        setShortDescription(
                    QObject::tr("Missing >NAME in the %1 package")
                    .arg(symbol->name()));
        setLongDescription(
                    QObject::tr("The %1 symbol does not have a >NAME text label on the tNames layer")
                    .arg(symbol->name()));
    }
}

void CheckSymbolNamesAndValuesResult::fix()
{
    if (!m_text)
        return;
    switch (m_type) {
    case CheckSymbolNamesAndValuesResult::Name:
        m_text->setLayer(EagleLayers::Names);
        break;
    case CheckSymbolNamesAndValuesResult::Value:
        m_text->setLayer(EagleLayers::Values);
        break;
    }
}

bool CheckSymbolNamesAndValuesResult::fixable() const
{
    return  m_text != nullptr;
}

