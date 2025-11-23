#include "test.h"


#include "mainwindow.h"

void Test_WithIpEdit::init(){
    ip_edit = new QLineEdit{m_text_to_init_editor};
    IntIpValidator::set_to(ip_edit);
}

void Test_WithIpEdit::cleanup(){
    delete ip_edit;
}

void Test_WithIpEdit::init_ip_edit(){
    const QString ip_edit_expected_value = m_text_to_init_editor;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

QString Test_WithIpEdit::ip_edit_actually_value(){
    Q_ASSERT(ip_edit != nullptr);
    return ip_edit->text();
}

int Test_WithIpEdit::ip_edit_actually_pos(){
    Q_ASSERT(ip_edit != nullptr);
    return ip_edit->cursorPosition();
}

void Test_WithIpEdit::start_edit(int cursor_pos){
    ip_edit->setFocus();

    const QString text = ip_edit_actually_value();
    const CursorPos cursor(text);
    if (cursor.is_valid(cursor_pos))
        ip_edit->setCursorPosition(cursor_pos);
}

void Test_WithIpEdit::finish_edit()
{
    QTest::keyClick(ip_edit, Qt::Key_Enter);
}


void Test_0_0_0_0::backspace_0xI0x0x0()
{
    start_edit(2);
    // 0.|0.0.0
    QTest::keyClick(ip_edit, backspace_key, kb_modifier, typing_delay);
    // 0|.0.0.0
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 1;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
}

void Test_0_0_0_0::backspace_0x0xI0x0()
{
    start_edit(4);
    // 0.0.|0.0
    QTest::keyClick(ip_edit, backspace_key, kb_modifier, typing_delay);
    // 0.0|.0.0
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 3;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
}

void Test_0_0_0_0::backspace_0x0x0xI0()
{
    start_edit(6);
    // 0.0.0.|0
    QTest::keyClick(ip_edit, backspace_key, kb_modifier, typing_delay);
    // 0.0.0|.0
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 5;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
}

void Test_0_0_0_0::delete_0Ix0x0x0()
{
    start_edit(1);
    // 0|.0.0.0
    QTest::keyClick(ip_edit, delete_key, kb_modifier, typing_delay);
    // 0.|0.0.0
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 2;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
}

void Test_0_0_0_0::delete_0x0Ix0x0()
{
    start_edit(3);
    // 0.0|.0.0
    QTest::keyClick(ip_edit, delete_key, kb_modifier, typing_delay);
    // 0.0.|0.0
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 4;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
}

void Test_0_0_0_0::delete_0x0x0Ix0()
{
    start_edit(5);
    // 0.0.0|.0
    QTest::keyClick(ip_edit, delete_key, kb_modifier, typing_delay);
    // 0.0.0.|0
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 6;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
}

void Test_0_0_0_0::backspace_0Ix0x0x0()
{

}

void Test_0_0_0_0::backspace_0x0Ix0x0()
{

}

void Test_0_0_0_0::backspace_0x0x0Ix0()
{

}

void Test_0_0_0_0::backspace_0x0x0x0I()
{

}

void Test_0_0_0_0::delete_I0x0x0x0()
{

}

void Test_0_0_0_0::delete_0xI0x0x0()
{

}

void Test_0_0_0_0::delete_0x0xI0x0()
{

}

void Test_0_0_0_0::delete_0x0x0xI0()
{

}

void Test_0_0_0_0::clear_all_by_backspace(){
    start_edit();
        // 0.0.0.0|
    {
        QTest::keyClick(ip_edit, backspace_key, kb_modifier, typing_delay);
        // 0.0.0.|
        const QString ip_edit_expected_value = "0.0.0.";
        const int ip_edit_expected_pos = 6;
        QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
        QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
    }
    {
        QTest::keyClick(ip_edit, backspace_key, kb_modifier, typing_delay);
        // 0.0.0|.
        const QString ip_edit_expected_value = "0.0.0.";
        const int ip_edit_expected_pos = 5;
        QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
        QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
    }
    {
        QTest::keyClick(ip_edit, backspace_key, kb_modifier, typing_delay);
        // 0.0.|.
        const QString ip_edit_expected_value = "0.0..";
        const int ip_edit_expected_pos = 4;
        QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
        QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
    }
    {
        QTest::keyClick(ip_edit, backspace_key, kb_modifier, typing_delay);
        // 0.0|..
        const QString ip_edit_expected_value = "0.0..";
        const int ip_edit_expected_pos = 3;
        QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
        QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
    }
    {
        QTest::keyClick(ip_edit, backspace_key, kb_modifier, typing_delay);
        // 0.|..
        const QString ip_edit_expected_value = "0...";
        const int ip_edit_expected_pos = 2;
        QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
        QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
    }
    {
        QTest::keyClick(ip_edit, backspace_key, kb_modifier, typing_delay);
        // 0|...
        const QString ip_edit_expected_value = "0...";
        const int ip_edit_expected_pos = 1;
        QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
        QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
    }
    {
        QTest::keyClick(ip_edit, backspace_key, kb_modifier, typing_delay);
        // |
        const QString ip_edit_expected_value = "";
        const int ip_edit_expected_pos = 0;
        QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
        QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
    }
    {
        finish_edit();
        // 0.0.0.0
        const QString ip_edit_state_after_finish_editin = "0.0.0.0";
        QCOMPARE(ip_edit_actually_value(), ip_edit_state_after_finish_editin);
    }
}

void Test_0_0_0_0::clear_all_by_delete()
{
    start_edit(0);
        // |0.0.0.0
    {
        QTest::keyClick(ip_edit, delete_key, kb_modifier, typing_delay);
        // |.0.0.0
        const QString ip_edit_expected_value = ".0.0.0";
        const int ip_edit_expected_pos = 0;
        QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
        QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
    }
    {
        QTest::keyClick(ip_edit, delete_key, kb_modifier, typing_delay);
        // .|0.0.0
        const QString ip_edit_expected_value = ".0.0.0";
        const int ip_edit_expected_pos = 1;
        QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
        QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
    }
    {
        QTest::keyClick(ip_edit, delete_key, kb_modifier, typing_delay);
        // .|.0.0
        const QString ip_edit_expected_value = "..0.0";
        const int ip_edit_expected_pos = 1;
        QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
        QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
    }
    {
        QTest::keyClick(ip_edit, delete_key, kb_modifier, typing_delay);
        // ..|0.0
        const QString ip_edit_expected_value = "..0.0";
        const int ip_edit_expected_pos = 2;
        QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
        QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
    }
    {
        QTest::keyClick(ip_edit, delete_key, kb_modifier, typing_delay);
        // ..|.0
        const QString ip_edit_expected_value = "...0";
        const int ip_edit_expected_pos = 2;
        QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
        QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
    }
    {
        QTest::keyClick(ip_edit, delete_key, kb_modifier, typing_delay);
        // ...|0
        const QString ip_edit_expected_value = "...0";
        const int ip_edit_expected_pos = 3;
        QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
        QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
    }
    {
        QTest::keyClick(ip_edit, delete_key, kb_modifier, typing_delay);
        // |
        const QString ip_edit_expected_value = "";
        const int ip_edit_expected_pos = 0;
        QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
        QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);
    }
    {
        finish_edit();
        // 0.0.0.0
        const QString ip_edit_state_after_finish_editin = "0.0.0.0";
        QCOMPARE(ip_edit_actually_value(), ip_edit_state_after_finish_editin);
    }
}

void Test_0_0_0_0::enter_1_to_0xI0x0x0()
{
    start_edit(2);
    // "0.|0.0.0"

    QTest::keyClick(ip_edit, Qt::Key_1, kb_modifier, typing_delay);
    // "0.1|0.0.0"
    const QString ip_edit_expected_value = "0.10.0.0";
    const int ip_edit_expected_pos = 3;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.10.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_0_to_0xI0x0x0()
{
    start_edit(2);
    // "0.|0.0.0"

    QTest::keyClick(ip_edit, Qt::Key_0, kb_modifier, typing_delay);
    // "0.0|.0.0"
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 3;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_0_to_0x0Ix0x0()
{
    start_edit(3);
    // "0.0|.0.0"

    QTest::keyClick(ip_edit, Qt::Key_0, kb_modifier, typing_delay);
    // "0.0|.0.0"
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 3;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

Test_0_0_0_0::Test_0_0_0_0(QObject *parent)
    : Test_WithIpEdit("0.0.0.0", parent)
{}

void Test_0_0_0_0::enter_0_to_I0x0x0x0()
{
    start_edit(0);
    // "|0.0.0.0"

    QTest::keyClick(ip_edit, Qt::Key_0, kb_modifier, typing_delay);
    // "0|.0.0.0"
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 1;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_0_to_0Ix0x0x0()
{
    start_edit(1);
    // "0|.0.0.0"

    QTest::keyClick(ip_edit, Qt::Key_0, kb_modifier, typing_delay);
    // "0|.0.0.0"
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 1;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_0_to_0x0x0xI0()
{
    start_edit(6);
    // "0.0.0.|0"

    QTest::keyClick(ip_edit, Qt::Key_0, kb_modifier, typing_delay);
    // "0.0.0.0|"
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 7;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_0_to_0x0x0x0I()
{
    start_edit(7);
    // "0.0.0.0|"

    QTest::keyClick(ip_edit, Qt::Key_0, kb_modifier, typing_delay);
    // "0.0.0.0|"
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 7;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_1_to_0x0Ix0x0()
{
    start_edit(3);
    // "0.0|.0.0"

    QTest::keyClick(ip_edit, Qt::Key_1, kb_modifier, typing_delay);
    // "0.1|.0.0"
    const QString ip_edit_expected_value = "0.1.0.0";
    const int ip_edit_expected_pos = 3;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.1.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_1_to_I0x0x0x0()
{
    start_edit(0);
    // "|0.0.0.0"

    QTest::keyClick(ip_edit, Qt::Key_1, kb_modifier, typing_delay);
    // "1|0.0.0.0"
    const QString ip_edit_expected_value = "10.0.0.0";
    const int ip_edit_expected_pos = 1;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "10.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_1_to_0Ix0x0x0()
{
    start_edit(1);
    // "0|.0.0.0"

    QTest::keyClick(ip_edit, Qt::Key_1, kb_modifier, typing_delay);
    // "1|.0.0.0"
    const QString ip_edit_expected_value = "1.0.0.0";
    const int ip_edit_expected_pos = 1;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "1.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_1_to_0x0x0xI0()
{
    start_edit(6);
    // "0.0.0.|0"

    QTest::keyClick(ip_edit, Qt::Key_1, kb_modifier, typing_delay);
    // "0.0.0.1|0"
    const QString ip_edit_expected_value = "0.0.0.10";
    const int ip_edit_expected_pos = 7;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.10"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_1_to_0x0x0x0I()
{
    start_edit(7);
    // "0.0.0.0|"

    QTest::keyClick(ip_edit, Qt::Key_1, kb_modifier, typing_delay);
    // "0.0.0.1|"
    const QString ip_edit_expected_value = "0.0.0.1";
    const int ip_edit_expected_pos = 7;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.1"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_dot_I0x0x0x0()
{
    QSKIP("Not Implemented");
}

void Test_0_0_0_0::enter_dot_0xI0x0x0()
{
    QSKIP("Not Implemented");
}

void Test_0_0_0_0::enter_dot_0x0xI0x0()
{
    QSKIP("Not Implemented");
}

void Test_0_0_0_0::enter_dot_0x0x0xI0()
{
    QSKIP("Not Implemented");
}

void Test_0_0_0_0::enter_comma_I0x0x0x0()
{
    QSKIP("Not Implemented");
}

void Test_0_0_0_0::enter_comma_0xI0x0x0()
{
    QSKIP("Not Implemented");
}

void Test_0_0_0_0::enter_comma_0x0xI0x0()
{
    QSKIP("Not Implemented");
}

void Test_0_0_0_0::enter_comma_0x0x0xI0()
{
    QSKIP("Not Implemented");
}

void Test_0_0_0_0::enter_space_I0x0x0x0()
{
    QSKIP("Not Implemented");
}

void Test_0_0_0_0::enter_space_0xI0x0x0()
{
    QSKIP("Not Implemented");
}

void Test_0_0_0_0::enter_space_0x0xI0x0()
{
    QSKIP("Not Implemented");
}

void Test_0_0_0_0::enter_space_0x0x0xI0()
{
    QSKIP("Not Implemented");
}

void Test_0_0_0_0::enter_dot_0x0Ix0x0()
{
    start_edit(3);
    // "0.0|.0.0"

    QTest::keyClick(ip_edit, '.', kb_modifier, typing_delay);
    // "0.0.|0.0"
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 4;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_comma_0x0Ix0x0()
{
    start_edit(3);
    // "0.0|.0.0"

    QTest::keyClick(ip_edit, ',', kb_modifier, typing_delay);
    // "0.0.|0.0"
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 4;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_space_0x0Ix0x0()
{
    start_edit(3);
    // "0.0|.0.0"

    QTest::keyClick(ip_edit, Qt::Key_Space, kb_modifier, typing_delay);
    // "0.0.|0.0"
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 4;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_dot_0Ix0x0x0()
{
    start_edit(1);
    // "0|.0.0.0"

    QTest::keyClick(ip_edit, '.', kb_modifier, typing_delay);
    // "0.|0.0.0"
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 2;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_comma_0Ix0x0x0()
{
    start_edit(1);
    // "0|.0.0.0"

    QTest::keyClick(ip_edit, ',', kb_modifier, typing_delay);
    // "0.|0.0.0"
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 2;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_space_0Ix0x0x0()
{
    start_edit(1);
    // "0|.0.0.0"

    QTest::keyClick(ip_edit, Qt::Key_Space, kb_modifier, typing_delay);
    // "0.|0.0.0"
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 2;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_dot_0x0x0Ix0()
{
    start_edit(5);
    // "0.0.0|.0"

    QTest::keyClick(ip_edit, '.', kb_modifier, typing_delay);
    // "0.0.0.|0"
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 6;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_comma_0x0x0Ix0()
{
    start_edit(5);
    // "0.0.0|.0"

    QTest::keyClick(ip_edit, ',', kb_modifier, typing_delay);
    // "0.0.0.|0"
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 6;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_0_0_0_0::enter_space_0x0x0Ix0()
{
    start_edit(5);
    // "0.0.0|.0"

    QTest::keyClick(ip_edit, Qt::Key_Space, kb_modifier, typing_delay);
    // "0.0.0.|0"
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 6;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void DirtyTestRunner::run_all()
{
    {
        auto test = Test_0_0_0_0();
        QTest::qExec(&test);
    }
    {
        auto test = Test_1_2_3_4();
        QTest::qExec(&test);
    }
    {
        auto test = Test_44_33_22_11();
        QTest::qExec(&test);
    }
    {
        auto test = Test_11_22_33_44();
        QTest::qExec(&test);
    }
    {
        auto test = Test_123_123_123_123();
        QTest::qExec(&test);
    }
    {
        auto test = Test_255_255_255_255();
        QTest::qExec(&test);
    }
    {
        auto test = Test_123_45_0_255();
        QTest::qExec(&test);
    }
}

void Test_1_2_3_4::enter_0_to_I1x2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_0_to_1Ix2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_0_to_1xI2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_0_to_1x2Ix3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_0_to_1x2x3xI4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_0_to_1x2x3x4I()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_1_to_I1x2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_1_to_1Ix2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_1_to_1xI2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_1_to_1x2Ix3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_1_to_1x2x3xI4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_1_to_1x2x3x4I()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_dot_1Ix2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_dot_1x2Ix3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_dot_1x2x3Ix4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_comma_1Ix2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_comma_1x2Ix3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_comma_1x2x3Ix4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_space_1Ix2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_space_1x2Ix3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_space_1x2x3Ix4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_dot_I1x2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_dot_1xI2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_dot_1x2xI3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_dot_1x2x3xI4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_comma_I1x2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_comma_1xI2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_comma_1x2xI3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_comma_1x2x3xI4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_space_I1x2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_space_1xI2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_space_1x2xI3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::enter_space_1x2x3xI4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::backspace_1xI2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::backspace_1x2xI3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::backspace_1x2x3xI4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::delete_1Ix2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::delete_1x2Ix3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::delete_1x2x3Ix4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::backspace_1Ix2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::backspace_1x2Ix3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::backspace_1x2x3Ix4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::backspace_1x2x3x4I()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::delete_I1x2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::delete_1xI2x3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::delete_1x2xI3x4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::delete_1x2x3xI4()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::clear_all_by_backspace()
{
    QSKIP("Not Implemented");
}

void Test_1_2_3_4::clear_all_by_delete()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_0_to_44x33xI22x11()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_0_to_44x33x22Ix11()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_0_to_44x33x22xI11()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_0_to_44x33x22x11I()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_0_to_44x33x2I2x11()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_0_to_44x33x22x1I1()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_1_to_44x33xI22x11()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_1_to_44x33x22Ix11()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_1_to_44x33x22xI11()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_1_to_44x33x22x11I()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_1_to_44x33x2I2x11()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_1_to_44x33x22x1I1()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_3_to_I44x33x22x11()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_0_to_4I4x33x22x11()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_0_to_44Ix33x22x11()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_3_to_44xI33x22x11()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_0_to_44x3I3x22x11()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::enter_0_to_44x33Ix22x11()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::clear_all_by_backspace()
{
    QSKIP("Not Implemented");
}

void Test_44_33_22_11::clear_all_by_delete()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_0_to_I11x22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_0_to_11Ix22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_0_to_11xI22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_0_to_11x22Ix33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_0_to_1I1x22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_0_to_11x2I2x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_1_to_I11x22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_1_to_11Ix22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_1_to_11xI22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_1_to_11x22Ix33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_1_to_1I1x22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_1_to_11x2I2x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_3_to_11x22xI33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_0_to_11x22x3I3x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_0_to_11x22x33Ix44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_3_to_11x22x33xI44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_0_to_11x22x33x4I4()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_0_to_11x22x33x44I()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_dot_to_11Ix22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_dot_to_11x22Ix33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_dot_to_11x22x33Ix44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_comma_to_11Ix22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_comma_to_11x22Ix33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_comma_to_11x22x33Ix44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_space_to_11Ix22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_space_to_11x22Ix33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_space_to_11x22x33Ix44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_dot_to_I11x22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_dot_to_11xI22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_dot_to_11x22xI33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_dot_to_11x22x33xI44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_comma_to_I11x22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_comma_to_11xI22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_comma_to_11x22xI33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_comma_to_11x22x33xI44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_space_to_I11x22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_space_to_11xI22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_space_to_11x22xI33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_space_to_11x22x33xI44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_dot_to_1I1x22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_dot_to_11x2I2x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_dot_to_11x22x3I3x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_dot_to_11x22x33x4I4()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_comma_to_1I1x22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_comma_to_11x2I2x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_comma_to_11x22x3I3x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_comma_to_11x22x33x4I4()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_space_to_1I1x22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_space_to_11x2I2x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_space_to_11x22x3I3x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::enter_space_to_11x22x33x4I4()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::backspace_11xI22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::backspace_11x22xI33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::backspace_11x22x33xI44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::delete_11Ix22x33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::delete_11x22Ix33x44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::delete_11x22x33Ix44()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::clear_all_by_backspace()
{
    QSKIP("Not Implemented");
}

void Test_11_22_33_44::clear_all_by_delete()
{
    QSKIP("Not Implemented");
}
