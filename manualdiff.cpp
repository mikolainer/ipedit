#include "manualdiff.h"
#include "ipv4.h"
#include "ipv4int.h"

TextEditState ManualDiff::fixup_inserted_separator(bool* is_processed) const
{
    TextEditState result = m_cur;
    const auto inserted_char = ch;
    const int inserted_index = index;
    
    if (!inserted || inserted_char != IpV4::octet_separator)
        return result;
    
    IpV4Int prev_ip{m_prev.val};
    const bool is_the_first_separator = prev_ip.is_valid();
    if (is_the_first_separator && !prev_ip.can_insert_first_separtor_to(inserted_index))
        throw std::runtime_error("can't fixup the input");
    
    auto it = result.val.begin() + inserted_index;
    *it = IpV4::octet_separator;
    if (is_the_first_separator || result.val == IpV4::octet_separator)
    {// have no separators
        ++it;
        
        while (result.val.count(IpV4::octet_separator) < IpV4::norm_separators_count)
        {
            int octet_val = 0;
            while (it != result.val.end())
            {
                const int growed_octet_val = (octet_val * 10) + (it->toLatin1() - '0');
                if (growed_octet_val > IpV4::Octet::max_value) break;
                it++;
                octet_val = growed_octet_val;
            }
            
            if (it == result.val.end())
            {
                result.val.append(IpV4::octet_separator);
                it = result.val.end();
            }
            else
            {
                const auto it_index = std::distance(result.val.begin(), it);
                result.val.insert(it_index, IpV4::octet_separator);
                it = result.val.begin() + it_index +1;
            }
        }
    }
    
    else if (inserted_index < result.val.lastIndexOf(IpV4::octet_separator))
    {// have separator to move
        ++it; // inserted separator leaved before
        while(it != result.val.end())
        {
            auto erased_char = *it;
            it = result.val.erase(it);
            if (erased_char == IpV4::octet_separator)
                break;
        }
    }
    else
    {// have no separator to move
        while(it != result.val.end())
            it = result.val.erase(it);
        --result.pos;
    }
    
    if (is_processed != nullptr)
        *is_processed = true;

    return result;
}

/* move the cursor (0.|0.0.0 -> 0.0|.0.0)
 * instead inserting invalid zero (0.|0.0.0 -> 0.0|0.0.0)
 */
TextEditState ManualDiff::fixup_inserted_zero(bool* is_processed) const
{
    TextEditState result = m_cur;
    
    if (!inserted || ch != '0')
        return result;
    
    const auto result_val_beg = result.val.begin();
    const auto result_val_end = result.val.end();

    const auto it = result_val_beg + index;
    const auto it_next = it+1;
    const auto it_next2 = it+2;

    if (    // is the first digit inserted to the octet
        (it == result_val_beg || *(it-1) == IpV4::octet_separator)
        &&  // and next digit is zero too
        (it_next != result_val_end && *it_next == '0')
        &&  // and next digit is last digit in the octet
        (it_next2 == result_val_end || *it_next2 == IpV4::octet_separator)
    )
        result.val.erase(it);
    
    if (is_processed != nullptr)
        *is_processed = true;
    return result;
}

bool ManualDiff::valid() const
{
    return index >= 0;
}

void ManualDiff::init_removed()
{
    const int removed_char_index = m_cur.pos;
    if (removed_char_index <= m_prev.val.length() -1)
    {
        const auto _removed_char = m_prev.val.at(removed_char_index);
        QString temp = QString(m_cur.val);
        temp.insert(m_cur.pos, _removed_char);
        if (temp == m_prev.val)
        {
            removed = true;
            index = removed_char_index;
            remove_dir = m_cur.pos != m_prev.pos ? Backward : Forward;
            ch = _removed_char;
        }
    }
}

void ManualDiff::init_inserted()
{
    const int inserted_char_index = m_prev.pos;
    if (inserted_char_index <= m_cur.val.length() -1)
    {
        const auto inserted_char = m_cur.val.at(inserted_char_index);
        QString temp(m_cur.val);
        temp.erase(temp.cbegin() + inserted_char_index);
        if (temp == m_prev.val)
        {
            inserted = true;
            index = inserted_char_index;
            ch = inserted_char;
        }
    }
}

ManualDiff::ManualDiff(const TextEditState &prev, const TextEditState &cur)
    : m_prev(prev), m_cur(cur)
{
    init_removed();
    init_inserted();
}

TextEditState ManualDiff::fixup_separators_count(bool* is_processed) const
{
    TextEditState result = m_cur;

    if (inserted)
        return fixup_inserted_separator();

    else if (removed && ch == IpV4::octet_separator)
    { // is separator removed
        result.val = m_prev.val;
        result.pos += remove_dir == Backward ? 0 : 1;
    }

    else if (removed && result.val == "...")
    { // is last digit removed
        result.val.clear();
    }

    if (is_processed != nullptr)
        *is_processed = true;

    return result;
}
