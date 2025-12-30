#include "ipv4int.h"

#include <QRegularExpression>

IpV4Int::IpV4Int(const QString &value) : m_text(value) {}

bool IpV4Int::is_valid() const
{
    bool ok = false;
    const unsigned long val = m_text.toULong(&ok);
    return ok && val <= max_value && !m_text.contains(QRegularExpression("\\D")) &&
           m_text != "" &&
           (!m_text.startsWith('0') || m_text == "0");
}

bool IpV4Int::is_invalid() const
{
    bool ok = false;
    const unsigned long val = m_text.toULong(&ok);

    return ok && !m_text.contains(QRegularExpression("\\D")) &&
           (val > max_value || (m_text.startsWith('0') && m_text != "0"));
}
