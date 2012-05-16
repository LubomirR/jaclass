#include "javaclass.h"
#include "javahelper.h"
#include <QDebug>

#include "javaconstantpool.h"

JavaClass::JavaClass()
{
    minorVer = 0;
    majorVer = 0;
    loaded = false;
}

JavaConstantPool & JavaClass::getConstantPool()
{
    return constantPool;
}

void JavaClass::load(const QString & fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "Could not open file.";
        return;
    }

    //QByteArray ba;

    QDataStream in(&file);
    in.setByteOrder(QDataStream::BigEndian);

    quint32 magic = 0;

    in >> magic;

    if (magic == 0xCAFEBABE) {
        in >> minorVer;
        in >> majorVer;
        in >> constantPoolCount;


        qDebug() << majorVer << minorVer;

        qDebug() << "Constant pool count:" << constantPoolCount;

        constantPool.loadFrom(in, constantPoolCount);

        /*int index = 1;
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



            //constantPool.push_back(c);

            constantPool[index] = c;
            index++;

            if (c.tag == CONSTANT_LONG || c.tag == CONSTANT_DOUBLE) {
                constant c;
                qMemSet(&c, 0, sizeof(c));
                c.tag = CONSTANT_INVALID;

                constantPool[index] = c;
                index++;
            }
        }*/


        //showConstantPool();

        //file.close();
        //return;

        in >> accessFlags;
        in >> thisClass;
        in >> superClass;
        in >> interfacesCount;

        for (quint32 i = 0; i < interfacesCount; i++) {
            quint16 interface = 0;
            in >> interface;
            interfaces.push_back(interface);
        }

        in >> fieldsCount;

        constantPool.show();

        file.close();
        loaded = true;
        return;

        for (int i = 0; i < fieldsCount; i++) {
            fieldInfo fi;
            fi.accessFlags = 0;
            fi.nameIndex = 0;
            fi.descriptorIndex = 0;
            fi.attributesCount = 0;
            //qMemSet(&fi, 0, sizeof(fi));

            in >> fi.accessFlags;
            in >> fi.nameIndex;
            in >> fi.descriptorIndex;
            in >> fi.attributesCount;

            for (quint32 j = 0; j < fi.attributesCount; j++) {
                attributeInfo ai;
                //qMemSet(&ai, 0, sizeof(ai));
                ai.attributeNameIndex = 0;
                ai.attributeLength = 0;
                in >> ai.attributeNameIndex;
                in >> ai.attributeLength;
                for (quint64 k = 0; k < ai.attributeLength; k++) {
                    quint8 info = 0;
                    in >> info;
                    ai.info.push_back(info);

                    //FIXME
                }

                fi.attributes.push_back(ai);
            }

            fields.push_back(fi);
        }

    } else {
        qDebug() << "Wrong header.";
    }

    file.close();
}

void JavaClass::unload()
{
    // TODO
    loaded = false;
}
