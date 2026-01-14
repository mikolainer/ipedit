#include "ipv4int.h"

#include <QRegularExpression>
#include "ipv4.h"

QString IpV4Int::to_ipv4() const
{
    QStringList result;

    bool ok;
    const unsigned long value = m_text.toULong(&ok);
    if (!ok) return m_text;

    for (int i = IpV4::norm_octets_count; i > 0; --i)
    {
        constexpr static const int octet_size = 8;
        constexpr static const int octet_mask = 0xFF;

        const int octet_index = i - 1;
        const int octet_shift = octet_index * octet_size;
        const int octet_val = (value & (octet_mask << octet_shift)) >> octet_shift;
        result.append(QString::number(octet_val));
    }

    return result.join(IpV4::octet_separator);
}

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

bool IpV4Int::can_insert_first_separtor_to(const int inserted_index) const
{
    if (!is_valid())
        return false;

    static const int octet_max_value_len = QString::number(IpV4::Octet::max_value).length();
    static const int max_len_of_text_after_first_dot = (IpV4::norm_octets_count -1) * octet_max_value_len;

    const int text_len_after_first_dot = m_text.length() - inserted_index;
    const QString text_before_first_dot = m_text.left(inserted_index);
    const ulong value_before_first_dot = text_before_first_dot.toULong();

    return
        text_len_after_first_dot <= max_len_of_text_after_first_dot
        &&
        value_before_first_dot <= IpV4::Octet::max_value;
}
