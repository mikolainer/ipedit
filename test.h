#ifndef TEST_H
#define TEST_H

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

class Test_WithIpEdit : public QObject
{ Q_OBJECT
public:
    constexpr static const int typing_delay = 200;
    constexpr static const Qt::KeyboardModifiers kb_modifier = Qt::NoModifier;
    constexpr static const Qt::Key backspace_key = Qt::Key_Backspace;
    constexpr static const Qt::Key delete_key = Qt::Key_Delete;

public:
    Test_WithIpEdit(QObject* parent = nullptr)
        : Test_WithIpEdit(default_text_to_init_editor, parent)
    {}

    Test_WithIpEdit(const QString& text_to_init_editor, QObject* parent = nullptr)
        : QObject(parent)
        , m_text_to_init_editor(text_to_init_editor)
    {};

private slots:
    void init();
    void cleanup();
    void init_ip_edit();

protected:
    QString ip_edit_actually_value();
    int ip_edit_actually_pos();

    void start_edit(int cursor_pos = CursorPos::invalid);
    void finish_edit();

public:
    constexpr static const char* default_text_to_init_editor{"0.0.0.0"};

protected:
    QLineEdit* ip_edit;

private:
    const QString m_text_to_init_editor;
};


class Test_RemoveChars_0_0_0_0 : public Test_WithIpEdit
{ Q_OBJECT
public:
    Test_RemoveChars_0_0_0_0(QObject* parent = nullptr)
        : Test_WithIpEdit("0.0.0.0", parent)
    {}

private slots:
    void remove_dot_by_backspace();
    void remove_dot_by_delete();
    void clear_all_by_backspace();
    void clear_all_by_delete();
};


class Test_EnterChars_0_0_0_0 : public Test_WithIpEdit
{ Q_OBJECT
public:
    Test_EnterChars_0_0_0_0(QObject* parent = nullptr)
        : Test_WithIpEdit("0.0.0.0", parent)
    {}

private slots:
    void enter_digit_after_dot();
    void enter_zero_before_zero_octet();
    void enter_zero_after_zero_octet();
    void enter_digit_after_zero_octet();
    void enter_dot_before_dot();
    void enter_comma_before_dot();
};

class Test_EnterChars_1_2_3_4 : public Test_WithIpEdit
{ Q_OBJECT
    constexpr static const int typing_delay = 200;
    constexpr static const Qt::KeyboardModifiers kb_modifier = Qt::NoModifier;

public:
    Test_EnterChars_1_2_3_4(QObject* parent = nullptr)
        : Test_WithIpEdit("1.2.3.4", parent)
    {}

private slots:
};

class Test_EnterChars_11_22_33_44 : public Test_WithIpEdit
{ Q_OBJECT
public:
    Test_EnterChars_11_22_33_44(QObject* parent = nullptr)
        : Test_WithIpEdit("11.22.33.44", parent)
    {}

private slots:
};

class Test_EnterChars_66_77_88_99 : public Test_WithIpEdit
{ Q_OBJECT
public:
    Test_EnterChars_66_77_88_99(QObject* parent = nullptr)
        : Test_WithIpEdit("66.77.88.99", parent)
    {}

private slots:
};

class Test_EnterChars_123_123_123_123 : public Test_WithIpEdit
{ Q_OBJECT
public:
    Test_EnterChars_123_123_123_123(QObject* parent = nullptr)
        : Test_WithIpEdit("123.123.123.123", parent)
    {}

private slots:
};

class DirtyTestRunner
{
public:
    DirtyTestRunner()
    {
        run_remove_tests();
        run_enter_tests();
    }

private:
    void run_remove_tests();
    void run_enter_tests();
};

#endif // TEST_H
