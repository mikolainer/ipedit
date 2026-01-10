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
    if (octets.length() != IpV4::norm_octets_count)
        return;

    const auto removed_indexes = remove_insignificant_zeros();
    shift_removed_pos(removed_indexes);
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
    return text.isEmpty() || IpV4(text).is_prevalid();
}

bool TextEditState::is_invalid() const
{
    const QString text = val;
    return IpV4Int(text).is_invalid();
}

QList<int> TextEditState::remove_insignificant_zeros()
{
    QList<int> removed_indexes;

    auto octets = val.split(IpV4::octet_separator);
    const QList<int> octet_removed_chars_cnt = [&octets](){
        QList<int> temp;
        for (QString& octet : octets)
            temp.append(IpV4::Octet::fix_start(octet));
        return temp;
    }();

    int octet_index = 0;
    QString::const_iterator it = val.cbegin();
    const QString::const_iterator the_end = val.cend();
    while (it != the_end)
    {
        if (*it == IpV4::octet_separator)
        {
            ++it;
            ++octet_index;
            continue;
        }

        int it_index_in_octet = 0;
        while(it != the_end && *it != IpV4::octet_separator)
        {
            if (it_index_in_octet < octet_removed_chars_cnt[octet_index])
                removed_indexes.append(std::distance(val.cbegin(), it));

            ++it_index_in_octet;
            ++it;
        }
    }

    val = octets.join(IpV4::octet_separator);
    return removed_indexes;
}

void TextEditState::shift_removed_pos(const QList<int> &removed_indexes)
{
    int shifted_count = 0;
    for (const int removed_char_index : removed_indexes)
    {
        if (pos + shifted_count <= removed_char_index)
            continue;

        ++shifted_count;
        --pos;
    }
}
