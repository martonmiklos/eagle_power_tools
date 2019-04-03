#include "libraryqachecksmodel.h"

#include "../../libraryqachecksregistry.h"

LibraryQASelectChecksModel::LibraryQASelectChecksModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    calculateGroupsCheckState();
}


void LibraryQASelectChecksModel::calculateSymbolsGroupCheckState()
{
    bool old = m_allSymbolCheckEnabled;
    m_allSymbolCheckEnabled = LibraryQAChecksRegistry::instance()->symbolChecks().count() != 0;
    for (LibraryQA_Check *check : LibraryQAChecksRegistry::instance()->symbolChecks()) {
        if (!check->isEnabled()) {
            m_allSymbolCheckEnabled = false;
            break;
        }
    }
    if (old != m_allSymbolCheckEnabled) {
        emit dataChanged(createIndex(LibraryQASelectElementsModel::Symbol, ColPerformCheck),
                         createIndex(LibraryQASelectElementsModel::Symbol, ColPerformCheck));
    }
}

void LibraryQASelectChecksModel::calculatePackagesGroupCheckState()
{
    bool old = m_allPackageCheckEnabled;
    m_allPackageCheckEnabled = LibraryQAChecksRegistry::instance()->packageChecks().count() != 0;
    for (LibraryQA_Check *check : LibraryQAChecksRegistry::instance()->packageChecks()) {
        if (!check->isEnabled()) {
            m_allPackageCheckEnabled = false;
            break;
        }
    }
    if (old != m_allSymbolCheckEnabled) {
        emit dataChanged(createIndex(LibraryQASelectElementsModel::Package, ColPerformCheck),
                         createIndex(LibraryQASelectElementsModel::Package, ColPerformCheck));
    }
}

void LibraryQASelectChecksModel::calculateDeviceSetsGroupCheckState()
{
    bool old = m_allDeviceSetCheckEnabled;
    m_allDeviceSetCheckEnabled = LibraryQAChecksRegistry::instance()->deviceSetChecks().count() != 0;
    for (LibraryQA_Check *check : LibraryQAChecksRegistry::instance()->deviceSetChecks()) {
        if (!check->isEnabled()) {
            m_allDeviceSetCheckEnabled = false;
            break;
        }
    }
    if (old != m_allSymbolCheckEnabled) {
        emit dataChanged(createIndex(LibraryQASelectElementsModel::Deviceset, ColPerformCheck),
                         createIndex(LibraryQASelectElementsModel::Deviceset, ColPerformCheck));
    }
}

void LibraryQASelectChecksModel::calculateGroupsCheckState()
{
    calculateSymbolsGroupCheckState();
    calculatePackagesGroupCheckState();
    calculateDeviceSetsGroupCheckState();
}

QModelIndex LibraryQASelectChecksModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (!parent.isValid()) {
        switch (static_cast<LibraryQASelectElementsModel::LibraryElementType>(row)) {
        case LibraryQASelectElementsModel::Symbol:
            return createIndex(row, column, LibraryQASelectElementsModel::SymbolsGroup);
        case LibraryQASelectElementsModel::Package:
            return createIndex(row, column, LibraryQASelectElementsModel::PackagesGroup);
        case LibraryQASelectElementsModel::Deviceset:
            return createIndex(row, column, LibraryQASelectElementsModel::DevicesetsGroup);
        }
    } else {
        switch (static_cast<LibraryQASelectElementsModel::TreeElementType>(parent.internalId())) {
        case LibraryQASelectElementsModel::SymbolsGroup:
            return createIndex(row, column, LibraryQASelectElementsModel::SymbolElement);
        case LibraryQASelectElementsModel::PackagesGroup:
            return createIndex(row, column, LibraryQASelectElementsModel::PackageElement);
        case LibraryQASelectElementsModel::DevicesetsGroup:
            return createIndex(row, column, LibraryQASelectElementsModel::DevicesetElement);
        default:
            return QModelIndex();
        }
    }
    return QModelIndex();
}

QModelIndex LibraryQASelectChecksModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    switch (static_cast<LibraryQASelectElementsModel::TreeElementType>(child.internalId())) {
    case LibraryQASelectElementsModel::TreeElementType::SymbolElement:
        return createIndex(LibraryQASelectElementsModel::Symbol, 0, LibraryQASelectElementsModel::SymbolsGroup);
    case LibraryQASelectElementsModel::TreeElementType::PackageElement:
        return createIndex(LibraryQASelectElementsModel::Package, 0, LibraryQASelectElementsModel::PackagesGroup);
    case LibraryQASelectElementsModel::TreeElementType::DevicesetElement:
        return createIndex(LibraryQASelectElementsModel::Deviceset, 0, LibraryQASelectElementsModel::DevicesetsGroup);
    default:
        return QModelIndex();
    }
}

QVariant LibraryQASelectChecksModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (!index.parent().isValid()) {
            if (index.column() == ColCheckName) {
                switch (static_cast<LibraryQASelectElementsModel::LibraryElementType>(index.row())) {
                case LibraryQASelectElementsModel::Symbol:
                    return tr("Symbol");
                case LibraryQASelectElementsModel::Package:
                    return tr("Footprint");
                case LibraryQASelectElementsModel::Deviceset:
                    return tr("Device");
                }
            }
        } else {
            if (index.column() == ColCheckName) {
                switch (static_cast<LibraryQASelectElementsModel::LibraryElementType>(index.parent().row())) {
                case LibraryQASelectElementsModel::Symbol:
                    return LibraryQAChecksRegistry::instance()->symbolChecks().at(index.row())->name();
                case LibraryQASelectElementsModel::Package:
                    return LibraryQAChecksRegistry::instance()->packageChecks().at(index.row())->name();
                case LibraryQASelectElementsModel::Deviceset:
                    return LibraryQAChecksRegistry::instance()->deviceSetChecks().at(index.row())->name();
                }
            }
        }
    } else if (role == Qt::CheckStateRole
               && index.column() == LibraryQASelectElementsModel::ColChecked) {
        if (index.parent().isValid()) {
            switch (static_cast<LibraryQASelectElementsModel::LibraryElementType>(index.parent().row())) {
            case LibraryQASelectElementsModel::Symbol:
                return LibraryQAChecksRegistry::instance()->symbolChecks().at(index.row())->isEnabled()
                        ? Qt::Checked : Qt::Unchecked;
            case LibraryQASelectElementsModel::Package:
                return LibraryQAChecksRegistry::instance()->packageChecks().at(index.row())->isEnabled()
                        ? Qt::Checked : Qt::Unchecked;
            case LibraryQASelectElementsModel::Deviceset:
                return LibraryQAChecksRegistry::instance()->deviceSetChecks().at(index.row())->isEnabled()
                        ? Qt::Checked : Qt::Unchecked;
            }
        } else {
            switch (static_cast<LibraryQASelectElementsModel::LibraryElementType>(index.row())) {
            case LibraryQASelectElementsModel::Symbol:
                return m_allSymbolCheckEnabled ? Qt::Checked : Qt::Unchecked;
            case LibraryQASelectElementsModel::Package:
                return m_allPackageCheckEnabled ? Qt::Checked : Qt::Unchecked;
            case LibraryQASelectElementsModel::Deviceset:
                return  m_allDeviceSetCheckEnabled ? Qt::Checked : Qt::Unchecked;
            }
        }
    }
    return QVariant();
}

bool LibraryQASelectChecksModel::setData(const QModelIndex &currentIndex, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole
            && currentIndex.column() == ColPerformCheck) {
        if (currentIndex.parent().isValid()) {
            switch (static_cast<LibraryQASelectElementsModel::LibraryElementType>(currentIndex.parent().row())) {
            case LibraryQASelectElementsModel::Symbol:
                LibraryQAChecksRegistry::instance()->symbolChecks().at(currentIndex.row())->setEnabled(value.toBool());
                calculateSymbolsGroupCheckState();
                break;
            case LibraryQASelectElementsModel::Package:
                LibraryQAChecksRegistry::instance()->packageChecks().at(currentIndex.row())->setEnabled(value.toBool());
                calculatePackagesGroupCheckState();
                break;
            case LibraryQASelectElementsModel::Deviceset:
                LibraryQAChecksRegistry::instance()->deviceSetChecks().at(currentIndex.row())->setEnabled(value.toBool());
                calculateDeviceSetsGroupCheckState();
                break;
            }
        } else {
            switch (static_cast<LibraryQASelectElementsModel::LibraryElementType>(currentIndex.row())) {
            case LibraryQASelectElementsModel::Symbol:
                for (LibraryQA_Check *check : LibraryQAChecksRegistry::instance()->symbolChecks())
                    check->setEnabled(value.toBool());
                m_allSymbolCheckEnabled = value.toBool();
                emit dataChanged(index(0, ColPerformCheck, currentIndex),
                                 index(LibraryQAChecksRegistry::instance()->symbolChecks().count() - 1, ColPerformCheck, currentIndex));
                break;
            case LibraryQASelectElementsModel::Package:
                for (LibraryQA_Check *check : LibraryQAChecksRegistry::instance()->packageChecks())
                    check->setEnabled(value.toBool());
                m_allPackageCheckEnabled = value.toBool();
                emit dataChanged(index(0, ColPerformCheck, currentIndex),
                                 index(LibraryQAChecksRegistry::instance()->packageChecks().count() - 1, ColPerformCheck, currentIndex));
                break;
            case LibraryQASelectElementsModel::Deviceset:
                for (LibraryQA_Check *check : LibraryQAChecksRegistry::instance()->deviceSetChecks())
                    check->setEnabled(value.toBool());
                m_allDeviceSetCheckEnabled = value.toBool();
                emit dataChanged(index(0, ColPerformCheck, currentIndex),
                                 index(LibraryQAChecksRegistry::instance()->deviceSetChecks().count() - 1, ColPerformCheck, currentIndex));
                break;
            }
            emit dataChanged(createIndex(currentIndex.row(), ColPerformCheck),
                             createIndex(currentIndex.row(), ColPerformCheck));
        }
    }
    return true;
}

QVariant LibraryQASelectChecksModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case ColCheckName:
            return tr("Check name");
        case ColPerformCheck:
            return tr("Perform");
        }
    }
    return QVariant();
}

Qt::ItemFlags LibraryQASelectChecksModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags ret = Qt::ItemIsEnabled;
    if (index.column() == LibraryQASelectElementsModel::ColChecked) {
        ret = Qt::ItemIsUserCheckable;
        if (!index.parent().isValid()) {
            switch (static_cast<LibraryQASelectElementsModel::LibraryElementType>(index.row())) {
            case LibraryQASelectElementsModel::Symbol:
                if (LibraryQAChecksRegistry::instance()->symbolChecks().count())
                    ret |= Qt::ItemIsEnabled;
                break;
            case LibraryQASelectElementsModel::Package:
                if (LibraryQAChecksRegistry::instance()->packageChecks().count())
                    ret |= Qt::ItemIsEnabled;
                break;
            case LibraryQASelectElementsModel::Deviceset:
                if (LibraryQAChecksRegistry::instance()->deviceSetChecks().count())
                    ret |= Qt::ItemIsEnabled;
                break;
            }
        } else {
            // separate checks
            ret |= Qt::ItemIsEnabled;
        }
    }
    return ret;
}

int LibraryQASelectChecksModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 3;
    else {
        switch (static_cast<LibraryQASelectElementsModel::TreeElementType>(parent.internalId())) {
        case LibraryQASelectElementsModel::SymbolsGroup:
            return  LibraryQAChecksRegistry::instance()->symbolChecks().count();
        case LibraryQASelectElementsModel::PackagesGroup:
            return LibraryQAChecksRegistry::instance()->packageChecks().count();
        case LibraryQASelectElementsModel::DevicesetsGroup:
            return LibraryQAChecksRegistry::instance()->deviceSetChecks().count();
        default:
            return 0;
        }
    }
}

int LibraryQASelectChecksModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

