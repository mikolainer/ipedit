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


void Test_RemoveChars_0_0_0_0::remove_dot_by_backspace()
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

void Test_RemoveChars_0_0_0_0::remove_dot_by_delete()
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

void Test_RemoveChars_0_0_0_0::clear_all_by_backspace(){
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

void Test_RemoveChars_0_0_0_0::clear_all_by_delete()
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

void Test_EnterChars_0_0_0_0::enter_digit_after_dot()
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

void Test_EnterChars_0_0_0_0::enter_zero_before_zero_octet()
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

void Test_EnterChars_0_0_0_0::enter_zero_after_zero_octet()
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

void Test_EnterChars_0_0_0_0::enter_digit_after_zero_octet()
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

void Test_EnterChars_0_0_0_0::enter_dot_before_dot()
{
    start_edit(3);
    // "0.0|.0.0"

    QTest::keyClick(ip_edit, '.', kb_modifier, typing_delay);
    // "0.1.|0.0"
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 4;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void Test_EnterChars_0_0_0_0::enter_comma_before_dot()
{
    start_edit(3);
    // "0.0|.0.0"

    QTest::keyClick(ip_edit, ',', kb_modifier, typing_delay);
    // "0.1.|0.0"
    const QString ip_edit_expected_value = "0.0.0.0";
    const int ip_edit_expected_pos = 4;
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
    QCOMPARE(ip_edit_actually_pos(), ip_edit_expected_pos);

    finish_edit();
    // "0.0.0.0"
    QCOMPARE(ip_edit_actually_value(), ip_edit_expected_value);
}

void DirtyTestRunner::run_remove_tests()
{
    {
        auto test = Test_RemoveChars_0_0_0_0();
        QTest::qExec(&test);
    }
}

void DirtyTestRunner::run_enter_tests()
{
    {
        auto test = Test_EnterChars_0_0_0_0();
        QTest::qExec(&test);
    }
}
