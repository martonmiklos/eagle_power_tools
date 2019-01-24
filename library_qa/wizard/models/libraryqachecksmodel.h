#ifndef LIBRARYQACHECKSMODEL_H
#define LIBRARYQACHECKSMODEL_H

#include "library_qa/libraryqacheck.h"

#include <QAbstractItemModel>

class LibraryQAChecksModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit LibraryQAChecksModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

signals:

public slots:
};

#endif // LIBRARYQACHECKSMODEL_H
