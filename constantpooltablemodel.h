#ifndef CONSTANTPOOLTABLEMODEL_H
#define CONSTANTPOOLTABLEMODEL_H

#include <QAbstractTableModel>
#include "javaclass.h"

class ConstantPoolTableModel : public QAbstractTableModel
{
    Q_OBJECT

    public:
        explicit ConstantPoolTableModel(JavaClass & jClass, QObject *parent = 0);
        virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
        virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
        virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    signals:

    public slots:

    private:
        JavaClass & javaClass;

};

#endif // CONSTANTPOOLTABLEMODEL_H
