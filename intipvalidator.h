#ifndef INTIPVALIDATOR_H
#define INTIPVALIDATOR_H

#include <QValidator>
#include <QLineEdit>

//class QLineEdit;

#include "ipv4int.h"
#include "ipv4.h"

class ManualDiff;

struct TextEditState{
    QString val;
    int pos;

    void fixup_wrong_separators()
    {
        for (const auto spec_ch : QString(IpV4::wrong_octet_separators))
            val.replace(spec_ch, IpV4::octet_separator);
    }

    void fixup_insignificant_zeros()
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

    void fixup_manual_changes(const TextEditState &prev);

    bool have_invalid_chars() const
    {
        const QString text = val;

        int available_char_count = text.count(IpV4::octet_separator);
        for (const auto digit : QString(IpV4::Octet::availavle_chars))
            available_char_count += text.count(digit);

        return available_char_count < text.length();
    }

    bool is_valid() const
    {
        const QString text = val;
        return IpV4(text).is_valid() || IpV4Int(text).is_valid();
    }

    bool is_prevalid() const
    {
        const QString text = val;
        return IpV4(text).is_prevalid();
    }

    bool is_invalid() const
    {
        const QString text = val;
        return IpV4Int(text).is_invalid();
    }
};

class ManualDiff{
public:
    enum RemoveDirection{
        Forward, // by Delete
        Backward // by Backspace
    };

    bool inserted = false;
    bool removed = false;
    RemoveDirection remove_dir = Forward;
    QChar ch = QChar::Null;
    int index = -1;

private:
    const TextEditState m_prev;
    const TextEditState m_cur;

public:
    ManualDiff(const TextEditState& prev, const TextEditState& cur)
        : m_prev(prev), m_cur(cur)
    {
        {
            const int removed_char_index = cur.pos;
            if (removed_char_index <= prev.val.length() -1)
            {
                const auto _removed_char = prev.val.at(removed_char_index);
                QString temp = QString(cur.val);
                temp.insert(cur.pos, _removed_char);
                if (temp == prev.val)
                {
                    removed = true;
                    index = removed_char_index;
                    remove_dir = cur.pos != prev.pos ? Backward : Forward;
                    ch = _removed_char;
                }
            }
        }

        {
            const int inserted_char_index = prev.pos;
            if (inserted_char_index <= cur.val.length() -1)
            {
                const auto inserted_char = cur.val.at(inserted_char_index);
                QString temp(cur.val);
                temp.erase(temp.cbegin() + inserted_char_index);
                if (temp == prev.val)
                {
                    inserted = true;
                    index = inserted_char_index;
                    ch = inserted_char;
                }
            }
        }
    }

    TextEditState fixup_removed_separator() const;
    TextEditState fixup_inserted_separator() const;
    TextEditState fixup_inserted_digit() const;
    TextEditState fixup_empty_field() const;

    bool is_insert_insignificant_zero() const;

    bool valid() const
    {
        return index > 0;
    }
};

class TextChangeHistory
{
public:
    TextChangeHistory(const QString &text = QString(), const int &pos = 0)
        :m_val({text}), m_pos({pos})
    {}

    bool update(const int &pos){
        update_text(cur_value());
        update_pos(pos);

        checkup();
        return true;
    }

    bool update(const QString &text, const int &pos){
        if (prev_value() == text && prev_pos() == pos)
            return false;

        update_text(text);
        update_pos(pos);

        checkup();
        return true;
    }

    bool update(const QLineEdit& edit){
        return update(edit.text(), edit.cursorPosition());
    }

    bool revert(){
        bool ok = true;

        if (m_pos.length() > 1)
            m_pos.removeFirst();
        else
            ok = false;

        if (m_val.length() > 1)
            m_val.removeFirst();
        else
            ok = false;

        return ok;
    }

    int prev_pos() const {
        return m_pos.length() > 1 ? m_pos[1] : m_pos[0];
    }

    QString prev_value() const {
        return m_val.length() > 1 ? m_val[1] : m_val[0];
    }

    int cur_pos() const {
        return m_pos[0];
    }

    QString cur_value() const {
        return m_val[0];
    }

private:
    void update_text(const QString &text)
    {
        m_val.prepend(text);

        if (m_val.length() > history_size)
            m_val.removeLast();
    }

    void update_pos(const int &pos)
    {
        m_pos.prepend(pos);

        if (m_pos.length() > history_size)
            m_pos.removeLast();
    }

    void checkup(){
        Q_ASSERT(m_pos.length() == m_val.length());
    }

private:
    QList<QString> m_val;
    QList<int> m_pos;

    constexpr static const int history_size {3};
};

class IpChangeAnalyzer
{
public:
    IpChangeAnalyzer(TextChangeHistory& history)
        : m_history(history)
    {}

    bool update_history(const TextEditState& cur)
    {
        const TextEditState prev{
            m_history.cur_value(),
            m_history.cur_pos()
        };

        return commit(prepare_new_state(prev, cur));
    }

    TextEditState prepare_new_state(const TextEditState& prev, const TextEditState& cur);

    bool commit(const TextEditState& cur) const{
        return m_history.update(cur.val, cur.pos);
    }

private:
    TextChangeHistory& m_history;
};

class IntIpValidator : public QValidator
{ Q_OBJECT

private:
    IntIpValidator(QObject* parent);

public:
    static void set_to(QLineEdit* editor = nullptr);

    virtual State validate(QString &text, int &pos) const override;
    virtual void fixup(QString &text) const override;

private:
    void update(const int &pos);
    void update(const QString &text, const int &pos);
    void update(const QLineEdit& edit);
    bool is_removed_manually(const QString &text, const int &pos, bool* is_backward_direction = nullptr, QChar* removed_char = nullptr) const;
    bool is_inserted_manually(const QString &text, QChar* inserted_char = nullptr) const;

private:
    TextChangeHistory m_text;
};

#endif // INTIPVALIDATOR_H
