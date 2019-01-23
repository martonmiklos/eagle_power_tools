#ifndef LIBRARYCOMPONENTSMODEL_H
#define LIBRARYCOMPONENTSMODEL_H

#include "eagle.h"

#include <QAbstractItemModel>

class LibraryElementsModel : public QAbstractItemModel
{
public:
    LibraryElementsModel(QObject *parent = nullptr);

    enum LibraryElementType {
        Sybmol,
        Package,
        DeviceSet
    };

    enum Columns {
        ColElementName,
        ColChecked
    };

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    void setLibrary(Library *lib);

private:
    Symbol::List m_symbolList;
    Package::List m_packages;
    Deviceset::List m_devicesets;
};

#endif // LIBRARYCOMPONENTSMODEL_H
