#ifndef ONECLICK_TESTFACTORY_H
#define ONECLICK_TESTFACTORY_H

#include <QString>
#include <QTest>
#include <QLineEdit>

class CursorPos{
public:
    CursorPos(const QString& text) : m_text(text) {}
    bool is_valid(int pos) const {
        return pos >= 0 && pos <= m_text.length();
    };
private:
    const QString m_text;
public:
    constexpr static const int invalid = -1;
};

class IpEditTestHelper : public QLineEdit
{ Q_OBJECT
    constexpr static const int typing_delay = 0;
    constexpr static const Qt::KeyboardModifiers kb_modifier = Qt::NoModifier;

public:
    IpEditTestHelper(const QString& text, QWidget* parent = nullptr);

    void start_edit(int cursor_pos = CursorPos::invalid);
    void finish_edit();

    void check(const QString& expected_text, int expected_pos);;
    void check(const QString& expected_finish_text);;
    void check(int expected_pos);

    void click(char key);
    void click(Qt::Key key);
};

struct State{
    QString value;
    int pos;

    QString toString() const;

    bool operator==(const State& other) const{
        return other.value == value
               && other.pos == pos;
    }
};

struct Click{
    Click(char key);
    Click(Qt::Key key);
    Click(bool is_char, Qt::Key key, char ch);
    Click();

    constexpr bool operator==(const Click& other) const{
        return other.is_char == is_char
               && other.click_char == click_char
               && other.click_key == click_key;
    }

    static char char_from_key(Qt::Key key);
    bool is_forward_remove() const;
    bool is_backward_remove() const;
    QString make_test_name(const State& start_state) const;
    QString operation_name() const;

public:
    bool is_char;
    Qt::Key click_key;
    char click_char;

    constexpr static const Qt::Key default_key{Qt::Key_Any};
    constexpr static const char default_char{' '};
};

struct Case{
    QString name;
    Click key;
    State start;
    State expected;
    QString finish_value;

    bool operator==(const Case& other) const{
        return other.name == name
               && other.key == key
               && other.start == start
               && other.expected == expected
               && other.finish_value == finish_value;
    }
};

class ClickEffect{
public:
    struct InputSet{
        constexpr static const char click_pos_marker{'|'};

        QString start_value;
        QList<int> click_pos;

        static const InputSet make(const QString& format);
        QList<State> start_states() const;
    };

public:
    ClickEffect(const Click& key) : m_key(key) {}

    virtual QList<Case> make(const InputSet& format) const;

protected:
    virtual State new_value(const State& start) const =0;
    virtual QString finish_value(const QString& expected) const =0;

    const Click m_key;
};

class Valid : public ClickEffect
{
public:
    Valid(const Click& key) : ClickEffect(key) {}

protected:
    State new_value(const State& start) const override;
    QString finish_value(const QString& expected) const override;
};

class Invalid : public ClickEffect
{
public:
    Invalid(const Click& key) : ClickEffect(key) {}

protected:
    State new_value(const State& start) const override;
    QString finish_value(const QString& expected) const override;
};

class OnlyPosMove : public ClickEffect
{
public:
    OnlyPosMove(const Click& key) : ClickEffect(key) {}

protected:
    State new_value(const State& start) const override;
    QString finish_value(const QString& expected) const override;
};

class InsertOver : public ClickEffect
{
public:
    InsertOver(const Click& key);;

protected:
    State new_value(const State& start) const override;
    QString finish_value(const QString& expected) const override;
};

class ReplaceBefore : public ClickEffect
{
public:
    ReplaceBefore(const Click& key);

protected:
    State new_value(const State& start) const override;
    QString finish_value(const QString& expected) const override;
};

class TestDataFactory
{
public:
    virtual void fill_data(const QList<Case>& tests, const QString* finish_value = nullptr);

    virtual void make_schema() const =0;
    virtual void fill_data(const Case& test) const =0;
};

class OneClickTestFactory : public TestDataFactory
{
public:
    OneClickTestFactory();
    void make_schema() const override;
    void fill_data(const Case& test) const override;
};

#endif // ONECLICK_TESTFACTORY_H
