#include "constantpooltablemodel.h"

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

    return javaClass.getConstantPool().size();
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
            if (row < javaClass.getConstantPool().size()) {
                if (col == 0) {
                    QList<quint16> keys = javaClass.getConstantPool().keys();
                    return keys[row];
                } else if (col == 1) {
                    QList<JavaClass::constant> values = javaClass.getConstantPool().values();
                    JavaClass::constant con = values[row];

                    switch (con.tag) {
                        case JavaClass::CONSTANT_INVALID:
                            return "(invalid)";
                        case JavaClass::CONSTANT_STRING:
                            return "String: '" + JavaClass::fromClassString(con.strdata, con.strlength) + "'";
                        case JavaClass::CONSTANT_INTEGER:
                            return con.intval;
                        case JavaClass::CONSTANT_FLOAT:
                            return con.floatval;
                        case JavaClass::CONSTANT_LONG:
                            return con.longval;
                        case JavaClass::CONSTANT_DOUBLE:
                            return con.doubleval;
                        case JavaClass::CONSTANT_CLASSREF:
                            return "Class ref: " + QString().setNum(con.ref);
                        case JavaClass::CONSTANT_STRINGREF:
                            return "String ref: " + QString().setNum(con.ref);
                        case JavaClass::CONSTANT_FIELDREF:
                            return "Field ref: " + QString().setNum(con.classref) + " " + QString().setNum(con.nametypedesc);
                        case JavaClass::CONSTANT_METHODREF:
                            return "Method ref: " + QString().setNum(con.classref) + " " + QString().setNum(con.nametypedesc);
                        case JavaClass::CONSTANT_INTMETHODREF:
                            return "Interface method ref: " + QString().setNum(con.classref) + " " + QString().setNum(con.nametypedesc);
                        case JavaClass::CONSTANT_NAMETYPEDESC:
                            return "Nametype desc: " + QString().setNum(con.namedesc) + " " + QString().setNum(con.typedesc);
                    }
                }
            }
            break;
    }

    return QVariant();
}
