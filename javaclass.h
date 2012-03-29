#ifndef JAVACLASS_H
#define JAVACLASS_H

#include <QFile>
#include <QMap>

class JavaClass
{
    public:
        JavaClass();
        void load(const QString & fileName);

        quint16 getMinorVer();
        quint16 getMajorVer();

    private:
        void showConstantPool();

        quint16 minorVer;
        quint16 majorVer;
        quint16 constantPoolCount;

        struct constant {
            quint8 tag;
            union {
                struct {
                    quint16 strlength;
                    unsigned char * strdata;
                };
                qint32 intval;
                float floatval;
                qint64 longval;
                double doubleval;
                quint16 ref;
                struct {
                    quint16 classref;
                    quint16 nametypedesc;
                };

                struct {
                    quint16 namedesc;
                    quint16 typedesc;
                };
            };
        };

        // This can't be a list because of Java engineers' stupid decision.
        QMap<quint16, constant> constantPool;

        quint16 accessFlags;
        quint16 thisClass;
        quint16 superClass;
        quint16 interfacesCount;
        QList<quint16> interfaces;
        quint16 fieldsCount;

        struct attributeInfo {
            quint16 attributeNameIndex;
            quint32 attributeLength;
            QList<quint8> info;
        };

        struct fieldInfo {
            quint16 accessFlags;
            quint16 nameIndex;
            quint16 descriptorIndex;
            quint16 attributesCount;
            QList<attributeInfo> attributes;
        };

        QList<fieldInfo> fields;

        struct methodInfo {
            quint16 accessFlags;
            quint16 nameIndex;
            quint16 descriptorIndex;
            quint16 attributesCount;
        };

        static const quint8 CONSTANT_INVALID = 0x00;
        static const quint8 CONSTANT_STRING = 0x01;
        static const quint8 CONSTANT_INTEGER = 0x03;
        static const quint8 CONSTANT_FLOAT = 0x04;
        static const quint8 CONSTANT_LONG = 0x05;
        static const quint8 CONSTANT_DOUBLE = 0x06;
        static const quint8 CONSTANT_CLASSREF = 0x07;
        static const quint8 CONSTANT_STRINGREF = 0x08;
        static const quint8 CONSTANT_FIELDREF = 0x09;
        static const quint8 CONSTANT_METHODREF = 0x0A;
        static const quint8 CONSTANT_INTMETHODREF = 0x0B;
        static const quint8 CONSTANT_NAMETYPEDESC = 0x0C;

        static const quint16 CLASS_ACC_PUBLIC = 0x0001;
        static const quint16 CLASS_ACC_FINAL = 0x0010;
        static const quint16 CLASS_ACC_SUPER = 0x0020;
        static const quint16 CLASS_ACC_INTERFACE = 0x0200;
        static const quint16 CLASS_ACC_ABSTRACT = 0x0400;
        static const quint16 CLASS_ACC_SYNTHETIC = 0x1000;
        static const quint16 CLASS_ACC_ANNOTATION = 0x2000;
        static const quint16 CLASS_ACC_ENUM = 0x4000;

        static const quint16 FIELD_ACC_PUBLIC = 0x0001;
        static const quint16 FIELD_ACC_PRIVATE = 0x0002;
        static const quint16 FIELD_ACC_PROTECTED = 0x0004;
        static const quint16 FIELD_ACC_STATIC = 0x0008;
        static const quint16 FIELD_ACC_FINAL = 0x0010;
        static const quint16 FIELD_ACC_VOLATILE = 0x0040;
        static const quint16 FIELD_ACC_TRANSIENT = 0x0080;
        static const quint16 FIELD_ACC_SYNTHETIC = 0x1000;
        static const quint16 FIELD_ACC_ENUM = 0x4000;

};

#endif // JAVACLASS_H
