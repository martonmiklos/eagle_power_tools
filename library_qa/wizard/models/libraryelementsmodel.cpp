#include "libraryelementssmodel.h"

LibraryElementsModel::LibraryElementsModel(QObject *parent) :
    QAbstractItemModel(parent)
{

}

QModelIndex LibraryElementsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();
    return createIndex(row, column, parent.row() + 1);
}

QModelIndex LibraryElementsModel::parent(const QModelIndex &child) const
{
    if (!child.isValid() || child.column() != 0 || child.internalId() == 0)
        return QModelIndex();

    return createIndex(child.column(), child.internalId() - 1);
}

QVariant LibraryElementsModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (!index.parent().isValid()) {
            if (index.column() == ColElementName) {
                switch (static_cast<LibraryElementType>(index.row())) {
                case LibraryElementsModel::Sybmol:
                    return tr("Symbol");
                    break;
                case LibraryElementsModel::Package:
                    return tr("Footprint");
                    break;
                case LibraryElementsModel::DeviceSet:
                    return tr("Device");
                    break;
                }
            }
        } else {
            if (index.column() == ColElementName) {
                switch (static_cast<LibraryElementType>(index.parent().row())) {
                case LibraryElementsModel::Sybmol:
                    return m_symbolList.at(index.row())->name();
                case LibraryElementsModel::Package:
                    return m_packages.at(index.row())->name();
                case LibraryElementsModel::DeviceSet:
                    return m_devicesets.at(index.row())->name();
                }
            }
        }
    } else if (role == Qt::CheckStateRole && index.column() == ColChecked) {
        return Qt::Checked; // TODO implement it
    }
    return QVariant();
}

bool LibraryElementsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return true;
}

QVariant LibraryElementsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("Element");
        case 1:
            return tr("Check");
        }
    }
    return QVariant();
}

Qt::ItemFlags LibraryElementsModel::flags(const QModelIndex &index) const
{
    if (index.parent().isValid() && index.column() == ColChecked) {
        return Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
    }
    return Qt::ItemIsEnabled;
}

int LibraryElementsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        switch (static_cast<LibraryElementType>(parent.row())) {
        case LibraryElementsModel::Sybmol:
            return m_symbolList.count();
        case LibraryElementsModel::Package:
            return m_packages.count();
        case LibraryElementsModel::DeviceSet:
            return m_devicesets.count();
        }
    }
    return 3;
}

int LibraryElementsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

void LibraryElementsModel::setLibrary(Library *lib)
{
    m_symbolList.clear();
    m_packages.clear();
    m_devicesets.clear();

    if (!lib)
        return;

    m_symbolList = *lib->symbols()->symbolList();
    m_packages = *lib->packages()->packageList();
    m_devicesets = *lib->devicesets()->devicesetList();
}
