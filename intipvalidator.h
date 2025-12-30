#ifndef INTIPVALIDATOR_H
#define INTIPVALIDATOR_H

#include <QValidator>
#include <QLineEdit>

//class QLineEdit;

class TextChangeTracker
{
public:
    TextChangeTracker(const QString &text = QString(), const int &pos = 0)
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

class IntIpValidator : public QValidator
{ Q_OBJECT

private:
    IntIpValidator(QObject* parent);

public:
    static void set_to(QLineEdit* editor = nullptr);

    virtual State validate(QString &text, int &pos) const override;
    virtual void fixup(QString &text) const override;

private:
    void fix_removed_separator(QString &text, int &pos, bool is_removing_direction_backward) const;
    void update(const int &pos);
    void update(const QString &text, const int &pos);
    void update(const QLineEdit& edit);
    bool is_removed_manually(const QString &text, const int &pos, bool* is_backward_direction = nullptr, QChar* removed_char = nullptr) const;
    bool is_inserted_manually(const QString &text, QChar* inserted_char = nullptr) const;

private:
    TextChangeTracker m_text;

public:
    constexpr static const char* available_chars {"1234567890"};
    constexpr static const char* special_chars {" .,/?ю"};
};

#endif // INTIPVALIDATOR_H
