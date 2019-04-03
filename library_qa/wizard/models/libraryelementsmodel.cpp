#include "libraryelementssmodel.h"


LibraryQASelectElementsModel::LibraryQASelectElementsModel(QObject *parent) :
    QAbstractItemModel(parent)
{

}

QModelIndex LibraryQASelectElementsModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (!parent.isValid()) {
        switch (static_cast<LibraryElementType>(row)) {
        case LibraryQASelectElementsModel::Symbol:
            return createIndex(row, column, SymbolsGroup);
        case LibraryQASelectElementsModel::Package:
            return createIndex(row, column, PackagesGroup);
        case LibraryQASelectElementsModel::Deviceset:
            return createIndex(row, column, DevicesetsGroup);
        }
    } else {
        switch (static_cast<TreeElementType>(parent.internalId())) {
        case LibraryQASelectElementsModel::SymbolsGroup:
            return createIndex(row, column, SymbolElement);
        case LibraryQASelectElementsModel::PackagesGroup:
            return createIndex(row, column, PackageElement);
        case LibraryQASelectElementsModel::DevicesetsGroup:
            return createIndex(row, column, DevicesetElement);
        default:
            return QModelIndex();
        }
    }
    return QModelIndex();
}

QModelIndex LibraryQASelectElementsModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    switch (static_cast<TreeElementType>(child.internalId())) {
    case TreeElementType::SymbolElement:
        return createIndex(Symbol, 0, SymbolsGroup);
    case TreeElementType::PackageElement:
        return createIndex(Package, 0, PackagesGroup);
    case TreeElementType::DevicesetElement:
        return createIndex(Deviceset, 0, DevicesetsGroup);
    default:
        return QModelIndex();
    }
}

QVariant LibraryQASelectElementsModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (!index.parent().isValid()) {
            if (index.column() == ColElementName) {
                switch (static_cast<LibraryElementType>(index.row())) {
                case LibraryQASelectElementsModel::Symbol:
                    return tr("Symbol");
                case LibraryQASelectElementsModel::Package:
                    return tr("Footprint");
                case LibraryQASelectElementsModel::Deviceset:
                    return tr("Device");
                }
            }
        } else {
            if (index.column() == ColElementName) {
                switch (static_cast<LibraryElementType>(index.parent().row())) {
                case LibraryQASelectElementsModel::Symbol:
                    return m_symbols.at(index.row())->name();
                case LibraryQASelectElementsModel::Package:
                    return m_packages.at(index.row())->name();
                case LibraryQASelectElementsModel::Deviceset:
                    return m_devicesets.at(index.row())->name();
                }
            }
        }
    } else if (role == Qt::CheckStateRole && index.column() == ColChecked) {
        return Qt::Checked; // TODO implement it
    }
    return QVariant();
}

bool LibraryQASelectElementsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.parent().isValid()
            && index.column() == ColChecked
            && role == Qt::CheckStateRole) {
        // TODO check!
        bool checked = value.toBool();
    }
    return true;
}

QVariant LibraryQASelectElementsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case ColElementName:
            return tr("Element");
        case ColChecked:
            return tr("Check");
        }
    }
    return QVariant();
}

Qt::ItemFlags LibraryQASelectElementsModel::flags(const QModelIndex &index) const
{
    if (index.isValid() && index.column() == ColChecked) {
        return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    }
    return Qt::ItemIsEnabled;
}

int LibraryQASelectElementsModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 3;
    else {
        switch (static_cast<TreeElementType>(parent.internalId())) {
        case LibraryQASelectElementsModel::SymbolsGroup:
            return  m_symbols.count();
        case LibraryQASelectElementsModel::PackagesGroup:
            return m_packages.count();
        case LibraryQASelectElementsModel::DevicesetsGroup:
            return  m_devicesets.count();
        default:
            return 0;
        }
    }
}

int LibraryQASelectElementsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

bool LibraryQASelectElementsModel::hasChildren(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return true;
    return GroupElementsBegin < parent.internalId() && parent.internalId() < GroupElementsEnd;
}

void LibraryQASelectElementsModel::setLibrary(Library *lib)
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
