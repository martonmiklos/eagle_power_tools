#include "libraryelementssmodel.h"


LibraryElementsModel::LibraryElementsModel(QObject *parent) :
    QAbstractItemModel(parent)
{

}

QModelIndex LibraryElementsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (!parent.isValid()) {
        switch (static_cast<LibraryElementType>(row)) {
        case LibraryElementsModel::Sybmol:
            return createIndex(row, column, SymbolsGroup);
        case LibraryElementsModel::Package:
            return createIndex(row, column, PackagesGroup);
        case LibraryElementsModel::Deviceset:
            return createIndex(row, column, DevicesetsGroup);
        }
    } else {
        switch (static_cast<TreeElementType>(parent.internalId())) {
        case LibraryElementsModel::SymbolsGroup:
            return createIndex(row, column, SymbolElement);
        case LibraryElementsModel::PackagesGroup:
            return createIndex(row, column, PackageElement);
        case LibraryElementsModel::DevicesetsGroup:
            return createIndex(row, column, DevicesetElement);
        default:
            return QModelIndex();
        }
    }
    return QModelIndex();
}

QModelIndex LibraryElementsModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    switch (static_cast<TreeElementType>(child.internalId())) {
    case TreeElementType::SymbolElement:
        return createIndex(Sybmol, 0, SymbolsGroup);
    case TreeElementType::PackageElement:
        return createIndex(Package, 0, PackagesGroup);
    case TreeElementType::DevicesetElement:
        return createIndex(Deviceset, 0, DevicesetsGroup);
    default:
        return QModelIndex();
    }
}

QVariant LibraryElementsModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (!index.parent().isValid()) {
            if (index.column() == ColElementName) {
                switch (static_cast<LibraryElementType>(index.row())) {
                case LibraryElementsModel::Sybmol:
                    return tr("Symbol");
                case LibraryElementsModel::Package:
                    return tr("Footprint");
                case LibraryElementsModel::Deviceset:
                    return tr("Device");
                }
            }
        } else {
            if (index.column() == ColElementName) {
                switch (static_cast<LibraryElementType>(index.parent().row())) {
                case LibraryElementsModel::Sybmol:
                    return m_symbols.at(index.row())->name();
                case LibraryElementsModel::Package:
                    return m_packages.at(index.row())->name();
                case LibraryElementsModel::Deviceset:
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
    if (index.parent().isValid()
            && index.column() == ColChecked
            && role == Qt::CheckStateRole) {
        // TODO check!
        bool checked = value.toBool();
    }
    return true;
}

QVariant LibraryElementsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
        switch (section) {
        case ColElementName:
            return tr("Element");
        case ColChecked:
            return tr("Check");
        }
    }
    return QVariant();
}

Qt::ItemFlags LibraryElementsModel::flags(const QModelIndex &index) const
{
    if (index.isValid() && index.column() == ColChecked) {
        return Qt::ItemIsUserCheckable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    }
    return Qt::ItemIsEnabled;
}

int LibraryElementsModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 3;
    else {
        switch (static_cast<TreeElementType>(parent.internalId())) {
        case LibraryElementsModel::SymbolsGroup:
            return  m_symbols.count();
        case LibraryElementsModel::PackagesGroup:
            return m_packages.count();
        case LibraryElementsModel::DevicesetsGroup:
            return  m_devicesets.count();
        default:
            return 0;
        }
    }
}

int LibraryElementsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

bool LibraryElementsModel::hasChildren(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return true;
    return GroupElementsBegin < parent.internalId() && parent.internalId() < GroupElementsEnd;
}

void LibraryElementsModel::setLibrary(Library *lib)
{
    beginResetModel();
    m_symbols.clear();
    m_packages.clear();
    m_devicesets.clear();

    if (!lib)
        return;

    m_symbols = *lib->symbols()->symbolList();
    m_packages = *lib->packages()->packageList();
    m_devicesets = *lib->devicesets()->devicesetList();
    endResetModel();
}
