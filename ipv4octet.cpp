#include "ipv4.h"

#include <QRegularExpression>

bool IpV4::Octet::fix_empty(QString &octet)
{
    bool changed = octet.isEmpty();
    if (octet.isEmpty())
    {
        octet = '0';
        changed = true;
    }
    return changed;
}

int IpV4::Octet::fix_start(QString &octet)
{
    int removed_char_count = 0;
    while (octet != '0' && octet.startsWith('0'))
    {
        octet.erase(octet.begin());
        ++removed_char_count;
    }
    return removed_char_count;
}

bool IpV4::Octet::is_valid(int value)
{
    return value >= min_value && value <= max_value;
}

bool IpV4::Octet::is_valid() const
{
    if (m_text.isEmpty())
        return false;

    if (m_text.contains(QRegularExpression("\\D")))
        return false;

    int value = int_value();
    if (!is_valid(value))
        return false;

    if (value > 0 && m_text.startsWith("0"))
        return false;

    if (value == 0 && m_text != "0")
        return false;

    return true;
}

int IpV4::Octet::int_value() const
{
    bool ok;
    const unsigned long value = m_text.toULong(&ok);
    return ok ? value : min_value-1;
}
