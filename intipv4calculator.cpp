#include "intipv4calculator.h"
#include "ipv4.h"
#include "ipv4int.h"

#include <QStringList>

QString IntIpV4Calculator::to_ipv4(const unsigned long int_ip)
{
    QStringList result;

    for (int i = IpV4::norm_octets_count; i > 0; --i)
    {
        constexpr static const int octet_size = 8;
        constexpr static const int octet_mask = 0xFF;

        const int octet_index = i - 1;
        const int octet_shift = octet_index * octet_size;
        const int octet_val = (int_ip & (octet_mask << octet_shift)) >> octet_shift;
        result.append(QString::number(octet_val));
    }

    return result.join(IpV4::octet_separator);
}

QString IntIpV4Calculator::to_ipv4(const QString &int_ip)
{
    bool ok;
    const unsigned long value = int_ip.toULong(&ok);
    if (!ok) return "0.0.0.0";

    return to_ipv4(value);
}

unsigned long IntIpV4Calculator::to_int(const QString &ip_v4)
{
    unsigned long result = 0;
    for (auto octet : ip_v4.split(IpV4::octet_separator))
    {
        result <<= 8;
        result += octet.toInt();
    }

    return result;
}

QString IntIpV4Calculator::convert(const QString &orig)
{
    if (IpV4Int(orig).is_valid())
        return to_ipv4(orig);

    const auto int_ip = to_int(IpV4::fix(orig));
    return QString("0x%1 (%2)").arg(
           QString::number(int_ip, 16).toUpper(),
           QString::number(int_ip, 10)
        );
}
