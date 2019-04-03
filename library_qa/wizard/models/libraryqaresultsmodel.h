#ifndef LIBRARYQARESULTSMODEL_H
#define LIBRARYQARESULTSMODEL_H

#include <QAbstractItemModel>

class LibraryQAResultsModel : public QAbstractItemModel
{
public:
    enum Columns {
        Col_Check,
        Col_Description,
        Col_Action,
        Col_Invalid
    };

    LibraryQAResultsModel(QObject *parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
};

#endif // LIBRARYQARESULTSMODEL_H
