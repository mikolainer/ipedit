#include "mainwindow.h"

#include "ipv4int.h"

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
                validator->update(validator->m_last_text, new_pos);
            }
        );

        const QString text = editor->text();
        const int pos = editor->cursorPosition();
        validator->update(text, pos);
    }
}

QValidator::State IntIpValidator::validate(QString &text, int &pos) const
{
    std::unique_ptr<QValidator::State> result;

    if (result.get() == nullptr)
    {
        if (text.isEmpty())
            result.reset(new QValidator::State(Intermediate));
    }

    if (result.get() == nullptr)
    {
        IpV4 ip{text};
        if (ip.is_valid())
            result.reset(new QValidator::State(Acceptable));
    }

    if (result.get() == nullptr)
    {
        IpV4Int ip{text};
        if (ip.is_valid())
            result.reset(new QValidator::State(Intermediate));

        else if (ip.is_invalid())
            result.reset(new QValidator::State(Invalid));
    }

    // fix separator removing (act as if moving the cursor)
    bool is_backward_removing = false;
    QChar removed_char = QChar::Null;
    const bool is_removed_manually =
        this->is_removed_manually(
            text, pos,
            &is_backward_removing,
            &removed_char
        );

    if (result.get() == nullptr && is_removed_manually && removed_char == octet_separator)
    {
        fix_removed_separator(text, pos, is_backward_removing);
        result.reset(new QValidator::State(Acceptable));
    }

    // fix invalid character enter
    QChar inserted_char = QChar::Null;
    const bool is_inserted_manually = this->is_inserted_manually(text, &inserted_char);
    if (result.get() == nullptr && is_inserted_manually
        && !QString(available_chars).contains(inserted_char)
        && !QString(special_chars).contains(inserted_char)
    )
    {
        result.reset(new QValidator::State(Invalid));
    }

    if (result.get() == nullptr && is_inserted_manually)
    {
        const int _inserted_index = m_last_pos;
        auto it = text.begin() + _inserted_index;

        if (QString(available_chars).contains(inserted_char))
        {
            // forward
            auto it_f = inserted_char == '0' ? it+1 : text.end();

            // backward
            auto it_b = (it == text.begin()) ? text.end() : it-1;

            if (it_b != text.end())
            {
                auto i = it_b;
                for(; *i == '0'; --i)
                    if (i == text.begin() || *i != '0') break;

                if (*i != '0' && *i != octet_separator)
                {
                    it_f = text.end();
                    it_b = text.end();
                }
            }

            while (it_f != text.end() || it_b != text.end())
            {
                if (*it_b != '0')
                    it_b = text.end();

                if (*it_f != '0')
                    it_f = text.end();

                if (it_f != text.end())
                {
                    pos++;
                    it_f = text.erase(it_f);
                }

                if (it_b != text.end())
                {
                    pos--;
                    it_b = text.erase(it_b);
                    it_b = (it_b == text.begin()) ? text.end() : it_b-1;
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
                IpV4Int last_ip{m_last_text};
                ulong val_before_first_dot = m_last_text.toULong();
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
                            m_last_text.length() - _inserted_index > max_ip_len_after_first_dot
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
                    result.reset(new QValidator::State(Invalid));
            }

            if (result.get() == nullptr)
            {
                *it = octet_separator;
                if (m_last_text.count(octet_separator) == 0)
                {// have no separators
                    ++it;

                    while (text.count(octet_separator) < norm_separators_count)
                    {
                        int octet_val = 0;
                        while (it != text.end())
                        {
                            const int growed_octet_val = (octet_val * 10) + (it->toLatin1() - '0');
                            if (growed_octet_val > max_octet_value) break;
                            it++;
                            octet_val = growed_octet_val;
                        }

                        if (it == text.end())
                        {
                            text.append(octet_separator);
                            it = text.end();
                        }
                        else
                        {
                            const auto it_index = std::distance(text.begin(), it);
                            text.insert(it_index, octet_separator);
                            it = text.begin() + it_index +1;
                        }
                    }

                    result.reset(new QValidator::State(Intermediate));
                }

                else if (_inserted_index < text.lastIndexOf(octet_separator))
                {// have separator to move
                    ++it; // inserted separator leaved before
                    while(it != text.end())
                    {
                        auto erased_char = *it;
                        it = text.erase(it);
                        if (erased_char == octet_separator)
                            break;
                    }

                    result.reset(new QValidator::State(Acceptable));
                }
                else
                {// have no separator to move
                   //result.reset(new QValidator::State(Invalid));  // reject the input

                    while(it != text.end())
                        it = text.erase(it);
                    --pos;
                }
            }
        }
    }

    // fix ip if there are octet starts with `0`
    {
        QStringList octets = text.split(octet_separator);
        QStringList octets_copy = octets;
        QList<int> removed_chars;
        QList<int> start_chars_cnt;
        for (QString& octet : octets)
        {
            start_chars_cnt.append(octet.length());
            removed_chars.append(IpV4::Octet::fix_start(octet));
        }

        if (octets.length() == norm_octets_count)
        {
            text = octets.join(octet_separator);

            int octet_start_pos = 0;
            for (int i = 0; i < norm_octets_count; ++i)
            {
                if (pos > octet_start_pos)
                    pos -= removed_chars[i];

                octet_start_pos += start_chars_cnt[i] - removed_chars[i] +1;
            }
        }
    }

//    if (result.get() == nullptr && text.count(octet_separator) == 0)
//    {
//        // TODO: fix it if max int value overflowed
//        result.reset(new QValidator::State(Intermediate));
//    }

    if (result.get() == nullptr && text.count(octet_separator) == norm_separators_count)
    {
        int empty_octets_count = 0;
        int valid_octets_count = 0;
        for (const QString& octet : text.split(octet_separator))
        {
            if (octet.isEmpty()) ++empty_octets_count;
            else{
                bool ok = false;
                const int octet_val = octet.toInt(&ok);
                if (ok && octet_val >= 0 && octet_val < 0x100)
                    ++valid_octets_count;
            }
        }

        if (empty_octets_count == norm_octets_count)
        {
            if (is_removed_manually)
                text.clear();

            result.reset(new QValidator::State(Intermediate));
        }

        else if (empty_octets_count + valid_octets_count == norm_octets_count)
            result.reset(new QValidator::State(Intermediate));

        else if (valid_octets_count == norm_octets_count)
            result.reset(new QValidator::State(Acceptable));
    }

    if (result.get() == nullptr)
        result.reset(new QValidator::State(Invalid));

    if (*result != Invalid)
        const_cast<IntIpValidator*>(this)->update(text, pos);

    return *result;
}

void IntIpValidator::fixup(QString &text) const {
    if (text.isEmpty())
    {
        text = "0.0.0.0";
        return;
    }

    const int separator_count = text.count(octet_separator);

    int fixes = 0;
    QStringList octets = text.split(octet_separator);
    for (QString& octet : octets){
        const bool empty_was_fixed = IpV4::Octet::fix_empty(octet);
        if (empty_was_fixed) {++fixes; continue;}
        const bool start_was_fixed = IpV4::Octet::fix_start(octet);
        if (start_was_fixed) {++fixes; continue;}
    }
    if (fixes > 0) {
        text = octets.join(octet_separator);
        return;
    }

    if (separator_count == 0)
        text = IpV4::from_int(text);
}

bool IntIpValidator::is_valid(const QString &text)
{
    const int separator_count = text.count(octet_separator);
    if (separator_count == 0 && (text.isEmpty() || !text.contains(R"(\D)")))
        return true;

    else if (separator_count == norm_separators_count)
    {
        for (const QString& octet : text.split(octet_separator))
            if (octet.isEmpty() || (octet.toInt() > max_octet_value))
                return false;

        return true;
    }

    return false;
}

bool IntIpValidator::is_invalid(const QString &text)
{
    return text.contains(R"([^ .,/?ю1234567890])");
}

void IntIpValidator::fix_removed_separator(QString &text, int &pos, bool is_removing_direction_backward) const
{
    constexpr int forvard_removing_pos_shift {1};
    constexpr int backward_removing_pos_shift {0};
    text = m_last_text;
    pos += is_removing_direction_backward ? backward_removing_pos_shift : forvard_removing_pos_shift ;
}

void IntIpValidator::update(const QString &text, const int &pos)
{
    m_last_pos = pos;
    m_last_text = text;
}

bool IntIpValidator::is_removed_manually(const QString &text, const int &pos, bool *is_backward_direction, QChar *removed_char) const
{
    const int _removed_char_index = pos;
    if (_removed_char_index > m_last_text.length() -1)
        return false;

    const auto _removed_char = m_last_text.at(_removed_char_index);
    const QString temp = QString(text).insert(pos, _removed_char);
    if (temp == m_last_text)
    {
        if(is_backward_direction)
            *is_backward_direction = pos != m_last_pos;
        if(removed_char)
            *removed_char = _removed_char;
        return true;
    }

    return false;
}

bool IntIpValidator::is_inserted_manually(const QString &text, QChar *inserted_char) const
{
    const int _inserted_char_index = m_last_pos;
    if (_inserted_char_index > text.length() -1)
        return false;

    const auto _inserted_char = text.at(_inserted_char_index);
    if (inserted_char)
        *inserted_char = _inserted_char;

    QString temp(text);
    temp.erase(temp.cbegin() + _inserted_char_index);
    return temp == m_last_text;
}

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

QString IpV4::from_int(const QString &int_text, bool* ok)
{
    QStringList result;

    bool _ok;
    const unsigned long value = int_text.toULong(&_ok);
    if (ok != nullptr)
        *ok = _ok;

    if (!_ok)
        return int_text;

    for (int i = IntIpValidator::norm_octets_count; i > 0; --i)
    {
        constexpr static const int octet_size = 8;
        constexpr static const int octet_mask = 0xFF;

        const int octet_index = i - 1;
        const int octet_shift = octet_index * octet_size;
        const int octet_val = (value & (octet_mask << octet_shift)) >> octet_shift;
        result.append(QString::number(octet_val));
    }

    return result.join(IntIpValidator::octet_separator);
}
