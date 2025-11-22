#include "mainwindow.h"

IntIpValidator::IntIpValidator(QObject *parent) : QValidator(parent) {}

void IntIpValidator::set_to(QLineEdit *editor){
    if (editor){
        auto validator = new IntIpValidator(editor);
        editor->setValidator(validator);
        QString text = editor->text();
        int pos = editor->cursorPosition();
        connect(
            editor, &QLineEdit::cursorPositionChanged,
            validator, [validator](int old_pos, int new_pos){
                validator->update(validator->m_last_text, new_pos);
            }
        );

        auto t_copy = text;
        auto p_copy = pos;
        do{
            if (validator->validate(text, pos) == Acceptable)
            {
                validator->update(text, pos);
                break;
            }
            else
            {
                validator->fixup(text);
                const int text_len = text.length();
                if (pos > text_len)
                    pos = text_len - 1;
                t_copy = text;
                p_copy = pos;
            }
        }
        while (t_copy != text || p_copy != pos);

        editor->setText(text);
    }
}

QValidator::State IntIpValidator::validate(QString &text, int &pos) const{
    std::unique_ptr<QValidator::State> result;
    if (IpV4(text).is_valid())
        result.reset(new QValidator::State(Acceptable));

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
                int octet_val = 0;
                bool dot_removed = false;
                while(it != text.end() && *it != octet_separator)
                {
                    octet_val = (octet_val * 10) + it->toLatin1() - '0';
                    if (octet_val > max_octet_value)
                        break;

                    it++;
                    if(!dot_removed && *it == octet_separator)
                    {
                        it = text.erase(it); // erase moved separator
                        dot_removed = true;
                    }
                }
                while(it != text.end() && *it != octet_separator)
                {
                    it = text.erase(it);
                }
                if (!dot_removed && *it == octet_separator && it != text.end())
                    it = text.erase(it); // erase moved separator

                result.reset(new QValidator::State(Acceptable));
            }
            else
            {// have no separator to move
                //text.erase(it); // erase moved separator
               result.reset(new QValidator::State(Invalid));  // reject the input
            }
        }
    }

    // fix ip if there are octet starts with `0`
    for (QString& octet : text.split(octet_separator)) // может не работать. TODO: проверить как рабоаете text.split(octet_separator)
        OctetFixup::fix_start(octet);

    if (result.get() == nullptr && text.count(octet_separator) == 0)
    {
        // TODO: fix it if max int value overflowed
        result.reset(new QValidator::State(Intermediate));
    }

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
            // TODO: fix ip if there are octet value overflowed
        }

        if (empty_octets_count == norm_octets_count)
        {
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
//    if ((separator_count != norm_separators_count) && (separator_count != 0))
//        text = default_valid_value;
//
//    bool fixed = false;
//    for (QString& octet : text.split(octet_separator)){
//        if (fixed = OctetFixup::fix_empty(octet)) continue;
//        if (fixed = OctetFixup::fix_start(octet)) continue;
//    }
//    if (fixed) return;
//
    if (separator_count == 0)
        TotalFixup::convert_from_int(text);
//    else
//    {}//TotalFixup::normalize_separators(text);
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
    return text.contains(R"([^., 1234567890])");
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

bool OctetFixup::fix_empty(QString &octet)
{
    bool changed = octet.isEmpty();
    if (octet.isEmpty())
    {
        octet = IntIpValidator::default_valid_octet;
        changed = true;
    }
    return changed;
}

bool OctetFixup::fix_start(QString &octet)
{
    bool changed = false;
    if (IntIpValidator::default_valid_octet == IntIpValidator::invalid_octet_start)
        while (octet != IntIpValidator::default_valid_octet && octet.startsWith(IntIpValidator::invalid_octet_start))
        {
            octet.erase(octet.begin());
            changed = true;
        }
    return changed;
}

void TotalFixup::convert_from_int(QString &text)
{
    QStringList result;

    bool ok;
    const int value = text.toInt(&ok);
    if (!ok) return;

    for (int i = IntIpValidator::norm_octets_count; i > 0; --i)
    {
        constexpr static const int octet_size = 8;
        constexpr static const int octet_mask = 0xFF;

        const int octet_index = i - 1;
        const int octet_shift = octet_index * octet_size;
        const int octet_val = (value & (octet_mask << octet_shift)) >> octet_shift;
        result.append(QString::number(octet_val));
    }

    text = result.join(IntIpValidator::octet_separator);
}

void TotalFixup::normalize_separators(QString &text)
{
    move_dots(text);
    strip_end(text);
}

void TotalFixup::move_dots(QString &text)
{
    QList<int> octets;
    for (const QString& octet_str : text.split(IntIpValidator::octet_separator))
        octets.append(octet_str.toInt());

    const int last_octet_index = octets.size() -1;
    int wrong_octet_index = 0;
    while (wrong_octet_index < last_octet_index)
        while (octets[wrong_octet_index] > IntIpValidator::max_octet_value)
        {
            int next_octet_new_digit = octets[wrong_octet_index] % 10;
            int& next_octet_value = octets[wrong_octet_index +1];
            const int next_octet_new_digit_ratio = [next_octet_value]() -> int {
                int result = 0;
                int next_octet_copy = next_octet_value;
                while(next_octet_copy > 0) {
                    result += (next_octet_copy % 10) > 0 ? 1:0;
                    next_octet_copy /= 10;
                }
                return result;
            }();

            next_octet_value += next_octet_new_digit * next_octet_new_digit_ratio;
        }

}

void TotalFixup::strip_end(QString &text)
{
    const qsizetype test_size = text.length();
    const qsizetype last_separator_pos = text.lastIndexOf(IntIpValidator::octet_separator);
    for (
        qsizetype octet_size = test_size - last_separator_pos;
        text.right(octet_size).toInt() > IntIpValidator::max_octet_value;
        --octet_size
    )
        text.erase(text.end() -1);
}
