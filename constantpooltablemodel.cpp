#include "constantpooltablemodel.h"
#include "javahelper.h"

ConstantPoolTableModel::ConstantPoolTableModel(JavaClass & jClass, QObject *parent) :
    QAbstractTableModel(parent),
    javaClass(jClass)
{
}

int ConstantPoolTableModel::rowCount(const QModelIndex & parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return javaClass.getConstantPool().getSize();
}

int ConstantPoolTableModel::columnCount(const QModelIndex & parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return 2;
}

QVariant ConstantPoolTableModel::data(const QModelIndex & index, int role) const
{

    int row = index.row();
    int col = index.column();

    switch(role) {
        case Qt::DisplayRole:
            if (row < javaClass.getConstantPool().getSize()) {
                quint16 index = javaClass.getConstantPool().numToIndex(row);
                const JavaConstantPool::constant & con = javaClass.getConstantPool().getConstantByIndex(index);

                if (col == 0) {
                    return JavaHelper::constantTypeToName(con.tag);
                } else if (col == 1) {
                    switch (con.tag) {
                        case JavaConstantPool::CONSTANT_INVALID:
                            return tr("(none)");
                        case JavaConstantPool::CONSTANT_STRING:
                            return JavaHelper::fromClassString(con.strdata, con.strlength);
                        case JavaConstantPool::CONSTANT_INTEGER:
                            return con.intval;
                        case JavaConstantPool::CONSTANT_FLOAT:
                            return con.floatval;
                        case JavaConstantPool::CONSTANT_LONG:
                            return con.longval;
                        case JavaConstantPool::CONSTANT_DOUBLE:
                            return con.doubleval;
                        case JavaConstantPool::CONSTANT_CLASSREF:
                        case JavaConstantPool::CONSTANT_STRINGREF:
                            if (javaClass.getConstantPool().isIndexValid(con.ref, JavaConstantPool::CONSTANT_STRING)) {
                                const JavaConstantPool::constant & cref = javaClass.getConstantPool().getConstantByIndex(con.ref);
                                return QString().setNum(con.ref) + " ('" + JavaHelper::fromClassString(cref.strdata, cref.strlength) + "')";
                            } else {
                                return QString().setNum(con.ref) + " (invalid)";
                            }

                        case JavaConstantPool::CONSTANT_FIELDREF:
                            return QString().setNum(con.classref) + " " + QString().setNum(con.nametypedesc);
                        case JavaConstantPool::CONSTANT_METHODREF:
                            return QString().setNum(con.classref) + " " + QString().setNum(con.nametypedesc);
                        case JavaConstantPool::CONSTANT_INTMETHODREF:
                            return QString().setNum(con.classref) + " " + QString().setNum(con.nametypedesc);
                        case JavaConstantPool::CONSTANT_NAMETYPEDESC:
                            {
                                QString name = "(invalid)";
                                QString type = "(invalid!";
                                if (javaClass.getConstantPool().isIndexValid(con.namedesc, JavaConstantPool::CONSTANT_STRING)) {
                                    const JavaConstantPool::constant & cref = javaClass.getConstantPool().getConstantByIndex(con.namedesc);
                                    name = JavaHelper::fromClassString(cref.strdata, cref.strlength);
                                }
                                if (javaClass.getConstantPool().isIndexValid(con.typedesc, JavaConstantPool::CONSTANT_STRING)) {
                                    const JavaConstantPool::constant & cref = javaClass.getConstantPool().getConstantByIndex(con.typedesc);
                                    type = JavaHelper::fromClassString(cref.strdata, cref.strlength);
                                }
                                return QString().setNum(con.namedesc) + " ('" + name + "') " + QString().setNum(con.typedesc) + "('" + type + "')";
                            }
                    }
                }
            }
            break;
    }

    return QVariant();
}

QVariant ConstantPoolTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
            switch (section) {
                case 0:
                    return tr("Type");
                case 1:
                    return tr("Value");
            }
        } else if (orientation == Qt::Vertical) {
            return javaClass.getConstantPool().numToIndex(section);
        }
    }

    return QVariant();
}
