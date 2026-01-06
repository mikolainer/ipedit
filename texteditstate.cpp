#include "texteditstate.h"

#include <QStringList>

#include "ipv4.h"
#include "ipv4int.h"
#include "manualdiff.h"

void TextEditState::fixup_wrong_separators()
{
    for (const auto spec_ch : QString(IpV4::wrong_octet_separators))
        val.replace(spec_ch, IpV4::octet_separator);
}

void TextEditState::fixup_insignificant_zeros()
{
    QStringList octets = val.split(IpV4::octet_separator);
    QStringList octets_copy = octets;
    QList<int> removed_chars;
    QList<int> start_chars_cnt;
    for (QString& octet : octets)
    {
        start_chars_cnt.append(octet.length());
        removed_chars.append(IpV4::Octet::fix_start(octet));
    }

    if (octets.length() == IpV4::norm_octets_count)
    {
        val = octets.join(IpV4::octet_separator);

        int octet_start_pos = 0;
        for (int i = 0; i < IpV4::norm_octets_count; ++i)
        {
            if (pos > octet_start_pos)
                pos -= removed_chars[i];

            octet_start_pos += start_chars_cnt[i] - removed_chars[i] +1;
        }
    }
}

void TextEditState::fixup_manual_changes(const TextEditState &prev)
{
    if (!ManualDiff(prev, *this).valid())
        return;
    
    *this = ManualDiff(prev, *this).fixup_removed_separator();
    *this = ManualDiff(prev, *this).fixup_inserted_separator();
    *this = ManualDiff(prev, *this).fixup_inserted_digit();
    *this = ManualDiff(prev, *this).fixup_empty_field();
}

bool TextEditState::have_invalid_chars() const
{
    const QString text = val;

    int available_char_count = text.count(IpV4::octet_separator);
    for (const auto digit : QString(IpV4::Octet::availavle_chars))
        available_char_count += text.count(digit);

    return available_char_count < text.length();
}

bool TextEditState::is_valid() const
{
    const QString text = val;
    return IpV4(text).is_valid() || IpV4Int(text).is_valid();
}

bool TextEditState::is_prevalid() const
{
    const QString text = val;
    return IpV4(text).is_prevalid();
}

bool TextEditState::is_invalid() const
{
    const QString text = val;
    return IpV4Int(text).is_invalid();
}
