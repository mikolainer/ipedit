
#include "intipvalidator.h"

#include <QLineEdit>

#include "ipv4int.h"
#include "ipv4.h"

IntIpValidator::IntIpValidator(QObject *parent) : QValidator(parent) {}

void IntIpValidator::set_to(QLineEdit *editor){
    if (editor){
        editor->clear();

        auto validator = new IntIpValidator(editor);
        editor->setValidator(validator);

        connect(
            editor, &QLineEdit::cursorPositionChanged,
            validator, [validator](int old_pos, int new_pos){
                Q_UNUSED(old_pos)
                validator->update(new_pos);
            }
        );

        validator->update(*editor);
    }
}

QValidator::State IntIpValidator::validate(QString &text, int &pos) const
{
    try{
        // WRONG!
        //if (ManualDiff({m_text.cur_value(), m_text.cur_pos()}, {text, pos}).is_insert_insignificant_zero())
        //    return Invalid;

        IpChangeAnalyzer(const_cast<TextChangeHistory&>(m_text)).update_history({text, pos});
        text = m_text.cur_value();
        pos = m_text.cur_pos();

        IpV4 ip{text};
        return ip.is_valid() ? Acceptable : Intermediate;
    }
    catch(const std::exception& e){
        return Invalid;
    }
}

void IntIpValidator::fixup(QString &text) const {
    if (text.isEmpty())
    {
        text = "0.0.0.0";
        return;
    }

    const IpV4Int int_ip = {text};
    if (int_ip.is_valid())
        text = int_ip.to_ipv4();
    else
        text = IpV4::fix(text);
}

TextEditState ManualDiff::fixup_inserted_separator() const
{
    TextEditState result = m_cur;
    const auto inserted_char = ch;
    const int inserted_index = index;

    if (!inserted || inserted_char != IpV4::octet_separator)
        return result;

    auto it = result.val.begin() + inserted_index;

    {
        IpV4Int last_ip{m_prev.val};
        ulong val_before_first_dot = m_prev.val.toULong();
        uint delim = 1;
        for (int i = inserted_index; i > 0; --i)
            delim *= 10;

        while (val_before_first_dot / delim > 0)
            val_before_first_dot /= 10;

        const int max_ip_len_after_first_dot = (IpV4::norm_octets_count -1) * QString::number(IpV4::Octet::max_value).length();
        if (
            (
                last_ip.is_valid()
                &&
                (
                    m_prev.val.length() - inserted_index > max_ip_len_after_first_dot
                    ||
                    inserted_index > QString::number(IpV4::Octet::max_value).length()
                    ||
                    (
                        inserted_index == QString::number(IpV4::Octet::max_value).length()
                        &&
                        val_before_first_dot > IpV4::Octet::max_value
                        )
                    )
                )
            )
            throw std::runtime_error("can't fixup the input");
    }


    *it = IpV4::octet_separator;
    if (m_prev.val.count(IpV4::octet_separator) == 0)
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
        //result.reset(new QValidator::State(Invalid));  // reject the input

        while(it != result.val.end())
            it = result.val.erase(it);
        --result.pos;
    }

    return result;
}

TextEditState ManualDiff::fixup_inserted_digit() const
{
    TextEditState result = m_cur;
    const auto inserted_char = ch;
    const int inserted_index = index;

    if (!inserted || inserted_char == IpV4::octet_separator)
        return result;

    auto it = result.val.begin() + inserted_index;

    // forward
    auto it_f = inserted_char == '0' ? it+1 : result.val.end();

    // backward
    auto it_b = (it == result.val.begin()) ? result.val.end() : it-1;

    if (it_b != result.val.end())
    {
        auto i = it_b;
        for(; *i == '0'; --i)
            if (i == result.val.begin() || *i != '0') break;

        if (*i != '0' && *i != IpV4::octet_separator)
        {
            it_f = result.val.end();
            it_b = result.val.end();
        }
    }

    while (it_f != result.val.end() || it_b != result.val.end())
    {
        if (*it_b != '0')
            it_b = result.val.end();

        if (*it_f != '0')
            it_f = result.val.end();

        if (it_f != result.val.end())
        {
            result.pos++;
            it_f = result.val.erase(it_f);
        }

        if (it_b != result.val.end())
        {
            result.pos--;
            it_b = result.val.erase(it_b);
            it_b = (it_b == result.val.begin()) ? result.val.end() : it_b-1;
        }
    }

    return result;
}

TextEditState ManualDiff::fixup_empty_field() const
{
    TextEditState result = m_cur;

    if (removed && result.val == "...")
        result.val.clear();

    return result;
}

bool ManualDiff::is_insert_insignificant_zero() const
{
    const auto zero_char = '0';

    if (!inserted || ch != zero_char)
        return false;

    int zeros_count = 1;
    auto it = m_cur.val.begin() + index +1;
    while (it != m_cur.val.end() && *it != zero_char)
        ++zeros_count;

    return zeros_count >0;
}

TextEditState ManualDiff::fixup_removed_separator() const
{
    TextEditState result = m_cur;
    if (removed && ch == IpV4::octet_separator)
    {
        result.val = m_prev.val;
        result.pos += remove_dir == Backward ? 1 : 0;
    }
    return result;
}

void IntIpValidator::update(const int &pos)
{
    m_text.update(pos);
}

void IntIpValidator::update(const QString &text, const int &pos)
{
    m_text.update(text, pos);
}

void IntIpValidator::update(const QLineEdit &edit)
{
    m_text.update(edit);
}

bool IntIpValidator::is_removed_manually(const QString &text, const int &pos, bool *is_backward_direction, QChar *removed_char) const
{
    const int _removed_char_index = pos;
    if (_removed_char_index > m_text.prev_value().length() -1)
        return false;

    const auto _removed_char = m_text.prev_value().at(_removed_char_index);
    const QString temp = QString(text).insert(pos, _removed_char);
    if (temp == m_text.prev_value())
    {
        if(is_backward_direction)
            *is_backward_direction = pos != m_text.prev_pos();
        if(removed_char)
            *removed_char = _removed_char;
        return true;
    }

    return false;
}

bool IntIpValidator::is_inserted_manually(const QString &text, QChar *inserted_char) const
{
    const int _inserted_char_index = m_text.prev_pos();
    if (_inserted_char_index > text.length() -1)
        return false;

    const auto _inserted_char = text.at(_inserted_char_index);
    if (inserted_char)
        *inserted_char = _inserted_char;

    QString temp(text);
    temp.erase(temp.cbegin() + _inserted_char_index);
    return temp == m_text.prev_value();
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

TextEditState IpChangeAnalyzer::prepare_new_state(const TextEditState &prev, const TextEditState &cur)
{
    TextEditState result = cur;
    result.fixup_wrong_separators();
    result.fixup_manual_changes(prev);
    result.fixup_insignificant_zeros();

    if (result.is_valid())
        return result;

    if (result.is_prevalid())
        return result;

    throw std::runtime_error("can't fixup the input");
}
