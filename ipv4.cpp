#include "ipv4.h"

#include <QStringList>

QString IpV4::fix(const QString &ip_text)
{
    int fixes = 0;
    QStringList octets = ip_text.split(IpV4::octet_separator);
    for (QString& octet : octets){
        const bool empty_was_fixed = IpV4::Octet::fix_empty(octet);
        if (empty_was_fixed) {++fixes; continue;}
        const bool start_was_fixed = IpV4::Octet::fix_start(octet);
        if (start_was_fixed) {++fixes; continue;}
    }
    return fixes > 0 ? octets.join(IpV4::octet_separator) : ip_text;
}

bool IpV4::is_valid() const
{
    int separators_count = m_text.count(octet_separator);
    if (separators_count != norm_separators_count)
        return false;

    for (const QString& octet : m_text.split(octet_separator))
        if (!Octet(octet).is_valid())
            return false;

    return true;
}

bool IpV4::is_prevalid() const
{
    int separators_count = m_text.count(octet_separator);
    if (separators_count != norm_separators_count)
        return false;

    for (const QString& octet : m_text.split(octet_separator))
        if (!Octet(octet).is_prevalid())
            return false;

    return true;
}
