#ifndef JAVACONSTANTPOOL_H
#define JAVACONSTANTPOOL_H

#include <QDataStream>
#include <QtGlobal>
#include <QMap>

class JavaConstantPool
{
    public:
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

        static const quint8 NUM_CONSTANT_TYPES = 11;

        static const quint8 constantTypes[NUM_CONSTANT_TYPES];

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

        JavaConstantPool();
        void loadFrom(QDataStream & in, quint16 count);
        constant & getConstantByIndex(int i);
        quint16 numToIndex(int num);
        int getSize() const;
        QList<quint16> getConstantIndices(quint8 tag = CONSTANT_INVALID) const;
        bool isIndexValid(quint16 index, quint8 tag = CONSTANT_INVALID) const;
        void show();
        QString getString(quint16 index);

    private:

        QMap<quint16, constant> getData();

        // This can't be a list because of Java engineers' stupid decision.
        QMap<quint16, constant> constantPool;

        quint16 constantPoolCount;
};

#endif // JAVACONSTANTPOOL_H
