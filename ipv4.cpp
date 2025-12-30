#include "ipv4.h"

#include <QStringList>

QString IpV4::from_int(const QString &int_text, bool* ok)
{
    QStringList result;
    
    bool _ok;
    const unsigned long value = int_text.toULong(&_ok);
    if (ok != nullptr)
        *ok = _ok;
    
    if (!_ok)
        return int_text;
    
    for (int i = norm_octets_count; i > 0; --i)
    {
        constexpr static const int octet_size = 8;
        constexpr static const int octet_mask = 0xFF;
        
        const int octet_index = i - 1;
        const int octet_shift = octet_index * octet_size;
        const int octet_val = (value & (octet_mask << octet_shift)) >> octet_shift;
        result.append(QString::number(octet_val));
    }
    
    return result.join(octet_separator);
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
