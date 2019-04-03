#include "libraryqaresultsmodel.h"

#include "library_qa/libraryqachecksregistry.h"

LibraryQAResultsModel::LibraryQAResultsModel(QObject *parent) :
    QAbstractItemModel(parent)
{

}

QModelIndex LibraryQAResultsModel::index(int row, int column, const QModelIndex &parent) const
{

}

QModelIndex LibraryQAResultsModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

QVariant LibraryQAResultsModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (!index.parent().isValid()) {
            if (index.column() == Col_Check) {
                switch (static_cast<LibraryQA_Check::CheckTarget>(index.row())) {
                case LibraryQA_Check::CheckTarget_Symbol:
                    return  tr("Symbol checks");
                case LibraryQA_Check::CheckTarget_Package:
                    return  tr("Package checks");
                case LibraryQA_Check::CheckTarget_Deviceset:
                    return  tr("Deviceset checks");
                }
            }
        } else {

        }
    }
    return QVariant();
}

QVariant LibraryQAResultsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (static_cast<Columns>(section)) {
        case LibraryQAResultsModel::Col_Check:
            return tr("Check");
        case LibraryQAResultsModel::Col_Description:
            return tr("Description");
        case LibraryQAResultsModel::Col_Action:
            return tr("Action");
        case Col_Invalid:
            return QVariant();
        }
    }
    return QVariant();
}

Qt::ItemFlags LibraryQAResultsModel::flags(const QModelIndex &index) const
{
    if (index.column() == Col_Action)
        return Qt::ItemIsEnabled | Qt::ItemIsEditable;
    return Qt::ItemIsEnabled;
}

int LibraryQAResultsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        switch (static_cast<LibraryQA_Check::CheckTarget>(parent.row())) {
        case LibraryQA_Check::CheckTarget_Symbol:
            break;
        case LibraryQA_Check::CheckTarget_Package:
            break;
        case LibraryQA_Check::CheckTarget_Deviceset:
            break;
        }
    }
    return 0;
}

int LibraryQAResultsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return Col_Invalid;
}
