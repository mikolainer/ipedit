#ifndef INTIPVALIDATOR_H
#define INTIPVALIDATOR_H

#include <QValidator>
#include <QLineEdit>

//class QLineEdit;

#include "ipv4int.h"
#include "ipv4.h"

struct TextEditState{
    QString val;
    int pos;
};

struct ManualDiff{
    enum RemoveDirection{
        Forward, // by Delete
        Backward // by Backspace
    };

    bool inserted = false;
    bool removed = true;
    RemoveDirection remove_dir = Forward;
    QChar ch = QChar::Null;
    int index = -1;

    ManualDiff(const TextEditState& prev, const TextEditState& cur){
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
                    remove_dir = cur.pos != prev.pos ? Backward : Forward;
                    ch = _removed_char;
                }
            }
        }

        {
            const int inserted_char_index = prev.pos;
            if (inserted_char_index <= cur.val.length() -1)
            {
                const auto _inserted_char = cur.val.at(inserted_char_index);
                QString temp(cur.val);
                temp.erase(temp.cbegin() + inserted_char_index);
                if (temp == prev.val)
                {
                    inserted = true;
                    ch = _inserted_char;
                }
            }
        }
    }

    bool is_valid()
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

    void fix_removed_separator(TextEditState& result, const ManualDiff& diff) const;

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

public:
    constexpr static const char* digit_chars {"1234567890"};
    constexpr static const char* special_chars {" .,/?ю"};
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
