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
    constexpr static const int typing_delay = 0;
    constexpr static const Qt::KeyboardModifiers kb_modifier = Qt::NoModifier;
    constexpr static const Qt::Key backspace_key = Qt::Key_Backspace;
    constexpr static const Qt::Key delete_key = Qt::Key_Delete;
    constexpr static const Qt::Key rarrow_key = Qt::Key_Right;
    constexpr static const Qt::Key larrow_key = Qt::Key_Left;

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


class Test_0_0_0_0 : public Test_WithIpEdit
{ Q_OBJECT
public:
    Test_0_0_0_0(QObject* parent = nullptr)
        : Test_WithIpEdit("0.0.0.0", parent)
    {}

private slots:
// insert zero around zero octet
    void enter_0_to_I0x0x0x0();
    void enter_0_to_0Ix0x0x0();
    void enter_0_to_0xI0x0x0();
    void enter_0_to_0x0Ix0x0();
    void enter_0_to_0x0x0xI0();
    void enter_0_to_0x0x0x0I();

// insert non zero digit around zero octet
    void enter_1_to_I0x0x0x0();
    void enter_1_to_0Ix0x0x0();
    void enter_1_to_0xI0x0x0();
    void enter_1_to_0x0Ix0x0();
    void enter_1_to_0x0x0xI0();
    void enter_1_to_0x0x0x0I();

// insert special char to jump over separator
    void enter_dot_0Ix0x0x0();
    void enter_dot_0x0Ix0x0();
    void enter_dot_0x0x0Ix0();

    void enter_comma_0Ix0x0x0();
    void enter_comma_0x0Ix0x0();
    void enter_comma_0x0x0Ix0();

    void enter_space_0Ix0x0x0();
    void enter_space_0x0Ix0x0();
    void enter_space_0x0x0Ix0();

private slots:
// remove dots
    void backspace_0xI0x0x0();
    void backspace_0x0xI0x0();
    void backspace_0x0x0xI0();

    void delete_0Ix0x0x0();
    void delete_0x0Ix0x0();
    void delete_0x0x0Ix0();

// remove zero octet
// remove all
    void clear_all_by_backspace();
    void clear_all_by_delete();
};

class Test_1_2_3_4 : public Test_WithIpEdit
{ Q_OBJECT
    constexpr static const int typing_delay = 200;
    constexpr static const Qt::KeyboardModifiers kb_modifier = Qt::NoModifier;

public:
    Test_1_2_3_4(QObject* parent = nullptr)
        : Test_WithIpEdit("1.2.3.4", parent)
    {}

private slots:
// insert zero around non zero octet
// insert non zero digit around non zero octet
// insert special char to jump over separator
// insert special char to make zero octet
// remove dots
// remove one digit non zero octet
// remove all
};

class Test_11_22_33_44 : public Test_WithIpEdit
{ Q_OBJECT
public:
    Test_11_22_33_44(QObject* parent = nullptr)
        : Test_WithIpEdit("11.22.33.44", parent)
    {}

private slots:
// insert zero around non zero octet
// insert zero between non zero digits
// insert non zero digit around non zero octet
// insert non zero digit between non zero digits
// insert to overflow octet value
// insert special char to jump over separator
// insert special char to make zero octet
// insert special char to cut end of octet
// remove dots
// remove all
};

class Test_123_123_123_123 : public Test_WithIpEdit
{ Q_OBJECT
public:
    Test_123_123_123_123(QObject* parent = nullptr)
        : Test_WithIpEdit("123.123.123.123", parent)
    {}

private slots:
// insert zero around full octet
// insert non zero digit around full octet
// insert special char to jump over separator
// insert special char to make zero octet
// insert special char to cut end of octet
// remove dots
// remove all
};

class Test_255_255_255_255 : public Test_WithIpEdit
{ Q_OBJECT
public:
    Test_255_255_255_255(QObject* parent = nullptr)
        : Test_WithIpEdit("255.255.255.255", parent)
    {}

private slots:
// insert zero around max octet
// insert non zero digit around max octet
// insert special char to jump over separator
// insert special char to make zero octet
// insert special char to cut end of octet
// remove all
};

class Test_123_45_0_255 : public Test_WithIpEdit
{ Q_OBJECT
public:
    Test_123_45_0_255(QObject* parent = nullptr)
        : Test_WithIpEdit("123.45.0.255", parent)
    {}

private slots:
    // move cursor by arrows
    // move cursor by arrows and enter
    // move cursor by arrows and remove
    // insert from clipboard
    // select and enter
    // select and insert from clipboard
};

class DirtyTestRunner
{
public:
    DirtyTestRunner()
    {
        run_all();
    }

private:
    void run_all();
};

#endif // TEST_H
