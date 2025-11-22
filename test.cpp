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
