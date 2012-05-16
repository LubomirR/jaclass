#include "javahelper.h"

#include <QDebug>

// Thanks for using Modified UTF-8 instead of UTF-8, Sun. I love writing this kind of functions.
QString JavaHelper::fromClassString(const unsigned char * str, unsigned int len) {
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

QString JavaHelper::constantTypeToName(quint8 type)
{
    switch (type) {
        case JavaConstantPool::CONSTANT_INVALID:
            return QObject::tr("(invalid)");
        case JavaConstantPool::CONSTANT_STRING:
            return QObject::tr("String");
        case JavaConstantPool::CONSTANT_INTEGER:
            return QObject::tr("Integer");
        case JavaConstantPool::CONSTANT_FLOAT:
            return QObject::tr("Float");
        case JavaConstantPool::CONSTANT_LONG:
            return QObject::tr("Long");
        case JavaConstantPool::CONSTANT_DOUBLE:
            return QObject::tr("Double");
        case JavaConstantPool::CONSTANT_CLASSREF:
            return QObject::tr("Class Reference");
        case JavaConstantPool::CONSTANT_STRINGREF:
            return QObject::tr("String Reference");
        case JavaConstantPool::CONSTANT_FIELDREF:
            return QObject::tr("Field Reference");
        case JavaConstantPool::CONSTANT_METHODREF:
            return QObject::tr("Method Reference");
        case JavaConstantPool::CONSTANT_INTMETHODREF:
            return QObject::tr("Interface Method Reference");
        case JavaConstantPool::CONSTANT_NAMETYPEDESC:
            return QObject::tr("Nametype Descriptor");
    }

    return "";
}
