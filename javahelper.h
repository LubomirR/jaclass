#ifndef JAVAHELPER_H
#define JAVAHELPER_H

#include <QString>

#include "javaconstantpool.h"

class JavaHelper
{

    public:
        static QString fromClassString(const unsigned char * str, unsigned int len);
        static QString constantTypeToName(quint8 type);

    private:
        JavaHelper() {}
};

#endif // JAVAHELPER_H
