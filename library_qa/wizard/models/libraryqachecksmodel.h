#ifndef LIBRARYQACHECKSMODEL_H
#define LIBRARYQACHECKSMODEL_H

#include "library_qa/libraryqacheck.h"
#include "library_qa/libraryqachecksregistry.h"
#include "libraryelementssmodel.h"

#include <QAbstractItemModel>

class LibraryQASelectChecksModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Columns {
        ColCheckName,
        ColPerformCheck
    };
    explicit LibraryQASelectChecksModel(QObject *parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

private:
    bool m_allSymbolCheckEnabled = true;
    bool m_allPackageCheckEnabled = true;
    bool m_allDeviceSetCheckEnabled = true;

    void calculateSymbolsGroupCheckState();
    void calculatePackagesGroupCheckState();
    void calculateGroupsCheckState();
    void calculateDeviceSetsGroupCheckState();
};

#endif // LIBRARYQACHECKSMODEL_H
