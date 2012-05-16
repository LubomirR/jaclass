#include "javaconstantpool.h"

#include <QDebug>
#include "javahelper.h"

const quint8 JavaConstantPool::constantTypes[JavaConstantPool::NUM_CONSTANT_TYPES] = {
    JavaConstantPool::CONSTANT_STRING,
    JavaConstantPool::CONSTANT_INTEGER,
    JavaConstantPool::CONSTANT_FLOAT,
    JavaConstantPool::CONSTANT_LONG,
    JavaConstantPool::CONSTANT_DOUBLE,
    JavaConstantPool::CONSTANT_CLASSREF,
    JavaConstantPool::CONSTANT_STRINGREF,
    JavaConstantPool::CONSTANT_FIELDREF,
    JavaConstantPool::CONSTANT_METHODREF,
    JavaConstantPool::CONSTANT_INTMETHODREF,
    JavaConstantPool::CONSTANT_NAMETYPEDESC
};

JavaConstantPool::JavaConstantPool()
{
}

void JavaConstantPool::loadFrom(QDataStream & in, quint16 count)
{
    constantPoolCount = count;

    int index = 1;
    for (int i = 0; i < constantPoolCount - 1; i++) {
        constant c;
        qMemSet(&c, 0, sizeof(c));
        c.tag = CONSTANT_INVALID;

        in >> c.tag;

        switch (c.tag) {
            case CONSTANT_STRING:
                in >> c.strlength;
                c.strdata = new unsigned char[c.strlength];
                in.readRawData((char *) c.strdata, c.strlength);

                break;

            case CONSTANT_INTEGER:
                in >> c.intval;

                break;

            case CONSTANT_FLOAT:
                in.setFloatingPointPrecision(QDataStream::SinglePrecision);
                in >> c.floatval;

                break;

            case CONSTANT_LONG:
                in >> c.longval;
                // "All 8-byte constants take up two entries in the constant_pool table of the class file."
                // I hate Java so much...
                i++;

                break;

            case CONSTANT_DOUBLE:
                in.setFloatingPointPrecision(QDataStream::DoublePrecision);
                in >> c.doubleval;
                // "All 8-byte constants take up two entries in the constant_pool table of the class file."
                // I hate Java so much...
                i++;

                break;

            case CONSTANT_CLASSREF:
            case CONSTANT_STRINGREF:
                in >> c.ref;

                break;

            case CONSTANT_FIELDREF:
            case CONSTANT_METHODREF:
            case CONSTANT_INTMETHODREF:
                in >> c.classref;
                in >> c.nametypedesc;

                break;

            case CONSTANT_NAMETYPEDESC:
                in >> c.namedesc;
                in >> c.typedesc;

                break;
            default:
                qDebug() << "ACHTUNG! ACHTUNG! ACHTUNG! HALT DIE KLAPPE!" << i << c.tag;
        }

        constantPool[index] = c;
        index++;

        if (c.tag == CONSTANT_LONG || c.tag == CONSTANT_DOUBLE) {
            //constant c;
            //qMemSet(&c, 0, sizeof(c));
            //c.tag = CONSTANT_INVALID;

            //constantPool[index] = c;
            index++;
        }
    }
}

JavaConstantPool::constant & JavaConstantPool::getConstantByIndex(int i)
{
    return constantPool[i];
}

quint16 JavaConstantPool::numToIndex(int num)
{
    QList<quint16> keys = constantPool.keys();
    if (num < keys.size()) {
        return keys[num];
    }

    return 0;
}

int JavaConstantPool::getSize() const
{
    return constantPool.size();
}

QList<quint16> JavaConstantPool::getConstantIndices(quint8 tag) const
{
    QList<quint16> ret;

    QMap<quint16, constant>::const_iterator i = constantPool.constBegin();
    while (i != constantPool.constEnd()) {
        if (tag == CONSTANT_INVALID || i.value().tag == tag) {
            ret.push_back(i.key());
        }
        ++i;
    }

    return ret;
}

bool JavaConstantPool::isIndexValid(quint16 index, quint8 tag) const
{
    /*if (index > constantPool.size()) {
        return false;
    }*/

    if (!constantPool.contains(index)) {
        return false;
    }

    constant con = constantPool[index];

    if (con.tag == CONSTANT_INVALID) {
        return false;
    }

    if (tag != CONSTANT_INVALID) {
        if (con.tag != tag) {
            return false;
        }
    }

    return true;
}

void JavaConstantPool::show()
{
    //TODO Replace with typedef.
    for (QMap<quint16, constant>::const_iterator i = constantPool.constBegin(); i != constantPool.constEnd(); i++) {
        constant c = i.value();

        qDebug() << "Constant" << i.key() << ":";

        switch(c.tag) {
            case CONSTANT_STRING:
                qDebug() << "CONSTANT_STRING";
                //qDebug() << QString::fromUtf8((const char *) c.strdata, c.strlength);
                qDebug() << JavaHelper::fromClassString(c.strdata, c.strlength);
                break;

            case CONSTANT_INTEGER:
                qDebug() << "CONSTANT_INTEGER";
                qDebug() << c.intval;
                break;

            case CONSTANT_FLOAT:
                qDebug() << "CONSTANT_FLOAT";
                qDebug() << c.floatval;
                break;

            case CONSTANT_LONG:
                qDebug() << "CONSTANT_LONG";
                qDebug() << c.longval;
                break;

            case CONSTANT_DOUBLE:
                qDebug() << "CONSTANT_DOUBLE";
                qDebug() << c.doubleval;
                break;

            case CONSTANT_CLASSREF:
                qDebug() << "CONSTANT_CLASSREF";
                qDebug() << c.ref;
                break;
            case CONSTANT_STRINGREF:
                qDebug() << "CONSTANT_STRINGREF";
                qDebug() << c.ref;
                break;

            case CONSTANT_FIELDREF:
                qDebug() << "CONSTANT_FIELDREF";
                qDebug() << c.classref << c.nametypedesc;
                break;

            case CONSTANT_METHODREF:
                qDebug() << "CONSTANT_METHODREF";
                qDebug() << c.classref << c.nametypedesc;
                break;

            case CONSTANT_INTMETHODREF:
                qDebug() << "CONSTANT_INTMETHODREF";
                qDebug() << c.classref << c.nametypedesc;
                break;

            case CONSTANT_NAMETYPEDESC:
                qDebug() << "CONSTANT_NAMETYPEDESC";
                qDebug() << c.namedesc << c.typedesc;
                break;
            default:
                qDebug() << "Unknown constant" << c.tag;
        }
    }
}

QString JavaConstantPool::getString(quint16 index)
{
    if (!isIndexValid(index)) {
        return QString("");
    }

    const constant & con = constantPool[index];

    if (con.tag != CONSTANT_STRING) {
        return QString("");
    }

    return JavaHelper::fromClassString(con.strdata, con.strlength);
}

QMap<quint16, JavaConstantPool::constant> JavaConstantPool::getData()
{
    return constantPool;
}

