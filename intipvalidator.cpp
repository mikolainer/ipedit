
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

void TextChangeHistory::fix_removed_separator(TextEditState& result, const ManualDiff& diff) const
{
    constexpr int forvard_removing_pos_shift {1};
    constexpr int backward_removing_pos_shift {0};
    result.val = prev_value();
    result.pos +=
        diff.removed && diff.remove_dir == ManualDiff::RemoveDirection::Backward ?
        backward_removing_pos_shift : forvard_removing_pos_shift;
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

TextEditState IpChangeAnalyzer::prepare_new_state(const TextEditState &prev, const TextEditState &cur)
{
    TextEditState result = cur;

    for (const auto spec_ch : QString(special_chars))
        result.val.replace(spec_ch, IpV4::octet_separator);

    {
        const QString text = result.val;
        int available_char_count = 0;
        for (const auto digit : QString(digit_chars))
            available_char_count += text.count(digit);

        available_char_count += text.count(IpV4::octet_separator);

        if (available_char_count < text.length())
            throw std::runtime_error("can't fixup the input");
    }

    const IpV4 ip{result.val};
    const IpV4Int int_ip{result.val};
    if (ip.is_valid() || int_ip.is_valid())
        return result;

    if (int_ip.is_invalid())
        throw std::runtime_error("can't fixup the input");

    const ManualDiff is_manual(prev, cur);

    if (is_manual.removed && is_manual.ch == IpV4::octet_separator)
    {
        m_history.fix_removed_separator(result, is_manual);
        return result;
    }

    if (is_manual.inserted)
    {
        const auto inserted_char = is_manual.ch;
        const int _inserted_index = is_manual.index;
        auto it = result.val.begin() + _inserted_index;

        if (QString(digit_chars).contains(inserted_char))
        {
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
        }

        /* fix special character enter
         *  - if input was an integer, insert separators up to normal count
         *  else
         *      - if entered in the last octet, clear it and all next characters
         *      - else move nearest right separator to entered position and clear all characters between
         */
        else if (QString(special_chars).contains(inserted_char))
        {
            {
                IpV4Int last_ip{m_history.prev_value()};
                ulong val_before_first_dot = m_history.prev_value().toULong();
                uint delim = 1;
                for (int i = _inserted_index; i > 0; --i)
                    delim *= 10;

                while (val_before_first_dot / delim > 0)
                    val_before_first_dot /= 10;

                const int max_ip_len_after_first_dot = (IpV4::norm_octets_count -1) * QString::number(IpV4::Octet::max_value).length();
                if (
                    (
                        last_ip.is_valid()
                        &&
                        (
                            m_history.prev_value().length() - _inserted_index > max_ip_len_after_first_dot
                            ||
                            _inserted_index > QString::number(IpV4::Octet::max_value).length()
                            ||
                            (
                                _inserted_index == QString::number(IpV4::Octet::max_value).length()
                                &&
                                val_before_first_dot > IpV4::Octet::max_value
                                )
                            )
                        )
                    )
                    throw std::runtime_error("can't fixup the input");
            }


            *it = IpV4::octet_separator;
            if (m_history.prev_value().count(IpV4::octet_separator) == 0)
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

                return result;
            }

            else if (_inserted_index < result.val.lastIndexOf(IpV4::octet_separator))
            {// have separator to move
                ++it; // inserted separator leaved before
                while(it != result.val.end())
                {
                    auto erased_char = *it;
                    it = result.val.erase(it);
                    if (erased_char == IpV4::octet_separator)
                        break;
                }

                return result;
            }
            else
            {// have no separator to move
                //result.reset(new QValidator::State(Invalid));  // reject the input

                while(it != result.val.end())
                    it = result.val.erase(it);
                --result.pos;

                return result;
            }
        }
    }

    // fix ip if there are octet starts with `0`
    {
        QStringList octets = result.val.split(IpV4::octet_separator);
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
            result.val = octets.join(IpV4::octet_separator);

            int octet_start_pos = 0;
            for (int i = 0; i < IpV4::norm_octets_count; ++i)
            {
                if (result.pos > octet_start_pos)
                    result.pos -= removed_chars[i];

                octet_start_pos += start_chars_cnt[i] - removed_chars[i] +1;
            }
        }
    }

    //    if (result.get() == nullptr && text.count(octet_separator) == 0)
    //    {
    //        // TODO: fix it if max int value overflowed
    //        result.reset(new QValidator::State(Intermediate));
    //    }

    if (result.val.count(IpV4::octet_separator) == IpV4::norm_separators_count)
    {
        int empty_octets_count = 0;
        int valid_octets_count = 0;
        for (const QString& octet : result.val.split(IpV4::octet_separator))
        {
            if (octet.isEmpty())
                ++empty_octets_count;
            else if (IpV4::Octet(octet).is_valid())
                ++valid_octets_count;
        }

        if (empty_octets_count == IpV4::norm_octets_count)
        {
            if (is_manual.removed)
                result.val.clear();

            return result;
        }

        else if (empty_octets_count + valid_octets_count == IpV4::norm_octets_count)
            return result;

        else if (valid_octets_count == IpV4::norm_octets_count)
            return result;
    }

    throw std::runtime_error("can't fixup the input");
}
