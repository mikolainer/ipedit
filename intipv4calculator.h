#ifndef INTIPV4CALCULATOR_H
#define INTIPV4CALCULATOR_H

#include <QString>

class IntIpV4Calculator
{
public:
    static QString to_ipv4(const unsigned long int_ip);
    static QString to_ipv4(const QString& int_ip);
    static unsigned long to_int(const QString& ip_v4);
    static QString convert(const QString &orig);
};

#endif // INTIPV4CALCULATOR_H
