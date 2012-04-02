#include "javaclass.h"
#include <QDebug>

JavaClass::JavaClass()
{
    minorVer = 0;
    majorVer = 0;
    loaded = false;
}

QMap<quint16, JavaClass::constant> & JavaClass::getConstantPool()
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
        }


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

        showConstantPool();

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

        showConstantPool();

    } else {
        qDebug() << "Wrong header.";
    }

    file.close();
}

void JavaClass::showConstantPool()
{
    //TODO Replace with typedef.
    for (QMap<quint16, constant>::const_iterator i = constantPool.constBegin(); i != constantPool.constEnd(); i++) {
        constant c = i.value();

        qDebug() << "Constant" << i.key() << ":";

        switch(c.tag) {
            case CONSTANT_STRING:
                qDebug() << "CONSTANT_STRING";
                //qDebug() << QString::fromUtf8((const char *) c.strdata, c.strlength);
                qDebug() << fromClassString(c.strdata, c.strlength);
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

// Thanks for using Modified UTF-8 instead of UTF-8, Sun. I love writing this kind of functions.
QString JavaClass::fromClassString(const unsigned char * str, unsigned int len) {
    unsigned int utf8len = 0;
    unsigned char * utf8str = new unsigned char[len];
    quint16 leadsur = 0;
    quint16 trailsur = 0;

    for (unsigned int i = 0; i < len;) {
        if (str[i] < 0x80) {
            // 7-bit ASCII character
            qDebug() << "ASCII character detected!";
            utf8str[utf8len] = str[i];
            utf8len++;
            i++;
        } else if ((str[i] >= 0xC0) && (str[i] <= 0xDF)) {
            // 2-byte sequence
            qDebug() << "2-byte sequence character detected!";
            unsigned char first = str[i];
            i++;
            //utf8len++;
            if (i < len) {
                //utf8str[i] = str[i];
                unsigned char second = str[i];
                i++;
                if (first == 0xC0 && second == 0x80) {
                    // Bastardized null character.
                    utf8str[utf8len++] = 0x00;
                    qDebug() << "Bastardized null character detected!";
                } else {
                    utf8str[utf8len++] = first;
                    utf8str[utf8len++] = second;
                }
            }
        } else if ((str[i] >= 0xE0) && (str[i] <= 0xEF)) {
            unsigned char first = str[i++];
            qDebug() << "3-byte sequence character detected!";

            if (i < len) {
                unsigned char second = str[i++];
                // We need a valid continuation byte (0b10000000).
                if (((second >= 0x80) && (second <= 0xBF)) && (i < len)) {
                    unsigned char third = str[i++];

                    // We need a valid continuation byte (0b10000000).
                    if ((third >= 0x80) && (third <= 0xBF)) {
                        qDebug() << "Got all bytes!";

                        quint16 val = 0;
                        val += (((quint16) third) & 0x3F);
                        val += ((((quint16) second) & 0x3F) << 6);
                        val += ((((quint16) first) & 0x0F) << 12);

                        if ((val >= 0xD800) && (val <= 0xDBFF)) {
                            // This is the lead surrogate.
                            leadsur = val;
                            qDebug() << "Decoded lead surrogate!";
                        } else if ((val >= 0xDC00) && (val <= 0xDFFF)) {
                            // This is the trail surrogate.

                            trailsur = val;
                            qDebug() << "Decoded trail surrogate!";

                            if (leadsur && trailsur) {
                                // Combine them into one character.

                                qDebug() << "Bastardized surrogate pair detected!";

                                quint32 totalval = 0;
                                totalval += (((quint32) trailsur) - 0xDC00);
                                totalval += ((((quint32) leadsur) - 0xD800) << 10);

                                totalval += 0x10000;

                                if (totalval <= 0x10FFFF) {
                                    // We don't want to support encoding of 5 and 6 byte sequences.

                                    utf8str[utf8len++] = 0xF0 | (unsigned char) ((totalval >> 18) & 0x07);
                                    utf8str[utf8len++] = 0x80 | (unsigned char) ((totalval >> 12) & 0x3F);
                                    utf8str[utf8len++] = 0x80 | (unsigned char) ((totalval >> 6) & 0x3F);
                                    utf8str[utf8len++] = 0x80 | (unsigned char) ((totalval) & 0x3F);
                                }
                            }

                            leadsur = 0;
                            trailsur = 0;

                        } else {
                            // Not a surrogate pair.
                            qDebug() << "Not a surrogate pair!" << val;
                            utf8str[utf8len++] = first;
                            utf8str[utf8len++] = second;
                            utf8str[utf8len++] = third;
                        }
                    }
                }
            }
        }
    }

    if (utf8len != 0) {
        return QString::fromUtf8((const char *) utf8str, utf8len);
    }

    return QString();
}

void JavaClass::unload()
{
    // TODO
    loaded = false;
}
