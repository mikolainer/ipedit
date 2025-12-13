#include "test_oneclick_testfactory.h"


Test_OneClickHelpers::Test_OneClickHelpers(QObject *parent) : QObject(parent) {}

void Test_OneClickHelpers::test_make_click_ch_space()
{
    char ch = ' ';
    Click actual = Click(ch);
    Click expected = Click{true, Click::default_key, ch};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_ch_dot()
{
    char ch = '.';
    Click actual = Click(ch);
    Click expected = Click{true, Click::default_key, ch};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_ch_0()
{
    char ch = '0';
    Click actual = Click(ch);
    Click expected = Click{true, Click::default_key, ch};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_ch_1()
{
    char ch = '1';
    Click actual = Click(ch);
    Click expected = Click{true, Click::default_key, ch};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_ch_2()
{
    char ch = '2';
    Click actual = Click(ch);
    Click expected = Click{true, Click::default_key, ch};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_ch_3()
{
    char ch = '3';
    Click actual = Click(ch);
    Click expected = Click{true, Click::default_key, ch};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_ch_4()
{
    char ch = '4';
    Click actual = Click(ch);
    Click expected = Click{true, Click::default_key, ch};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_ch_5()
{
    char ch = '5';
    Click actual = Click(ch);
    Click expected = Click{true, Click::default_key, ch};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_ch_6()
{
    char ch = '6';
    Click actual = Click(ch);
    Click expected = Click{true, Click::default_key, ch};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_ch_7()
{
    char ch = '7';
    Click actual = Click(ch);
    Click expected = Click{true, Click::default_key, ch};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_ch_8()
{
    char ch = '8';
    Click actual = Click(ch);
    Click expected = Click{true, Click::default_key, ch};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_ch_9()
{
    char ch = '9';
    Click actual = Click(ch);
    Click expected = Click{true, Click::default_key, ch};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_ch_comma()
{
    char ch = ',';
    Click actual = Click(ch);
    Click expected = Click{true, Click::default_key, ch};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_key_space()
{
    Qt::Key key = Qt::Key_Space;
    Click actual(key);
    Click expected{false, key, Click::default_char};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_key_0()
{
    Qt::Key key = Qt::Key_0;
    Click actual(key);
    Click expected{false, key, '0'};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_key_1()
{
    Qt::Key key = Qt::Key_1;
    Click actual(key);
    Click expected{false, key, '1'};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_key_2()
{
    Qt::Key key = Qt::Key_2;
    Click actual(key);
    Click expected{false, key, '2'};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_key_3()
{
    Qt::Key key = Qt::Key_3;
    Click actual(key);
    Click expected{false, key, '3'};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_key_4()
{
    Qt::Key key = Qt::Key_4;
    Click actual(key);
    Click expected{false, key, '4'};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_key_5()
{
    Qt::Key key = Qt::Key_5;
    Click actual(key);
    Click expected{false, key, '5'};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_key_6()
{
    Qt::Key key = Qt::Key_6;
    Click actual(key);
    Click expected{false, key, '6'};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_key_7()
{
    Qt::Key key = Qt::Key_7;
    Click actual(key);
    Click expected{false, key, '7'};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_key_8()
{
    Qt::Key key = Qt::Key_8;
    Click actual(key);
    Click expected{false, key, '8'};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_click_key_9()
{
    Qt::Key key = Qt::Key_9;
    Click actual(key);
    Click expected{false, key, '9'};
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_input_set()
{
    auto actual = ClickEffect::InputSet::make("|1.|1|.1|.1|");
    const QString expected_value{"1.1.1.1"};
    const QList<int> expected_pos_set{0,2,3,5,7};

    QCOMPARE(actual.start_value, expected_value);
    QCOMPARE(actual.click_pos, expected_pos_set);
}

void Test_OneClickHelpers::test_make_valid_cases_insert()
{
    auto test_factory = Valid(Click(Qt::Key_0));
    auto actual = test_factory.make(ClickEffect::InputSet::make("1|.1|.1.1"));
    QCOMPARE(actual.count(), 2);
    QCOMPARE((actual[0].start), (State{"1.1.1.1", 1}));
    QCOMPARE((actual[0].expected), (State{"10.1.1.1", 2}));
    QCOMPARE((actual[0].finish_value), ("10.1.1.1"));
    QCOMPARE((actual[1].start), (State{"1.1.1.1", 3}));
    QCOMPARE((actual[1].expected), (State{"1.10.1.1", 4}));
    QCOMPARE((actual[1].finish_value), ("1.10.1.1"));
}

void Test_OneClickHelpers::test_make_valid_cases_remove_forward()
{
    auto test_factory = Valid(Click(Qt::Key_Delete));
    auto actual = test_factory.make(ClickEffect::InputSet::make("1|2.12.12.12"));
    QCOMPARE(actual.count(), 1);
    QCOMPARE((actual[0].start), (State{"12.12.12.12", 1}));
    QCOMPARE((actual[0].expected), (State{"1.12.12.12", 1}));
    QCOMPARE((actual[0].finish_value), ("1.12.12.12"));
}

void Test_OneClickHelpers::test_make_valid_cases_remove_backward()
{
    auto test_factory = Valid(Click(Qt::Key_Backspace));
    auto actual = test_factory.make(ClickEffect::InputSet::make("12|.12.12.12"));
    QCOMPARE(actual.count(), 1);
    QCOMPARE((actual[0].start), (State{"12.12.12.12", 2}));
    QCOMPARE((actual[0].expected), (State{"1.12.12.12", 1}));
    QCOMPARE((actual[0].finish_value), ("1.12.12.12"));
}

void Test_OneClickHelpers::test_make_invalid_cases_insert()
{
    auto test_factory = Invalid(Click(Qt::Key_0));
    auto actual = test_factory.make(ClickEffect::InputSet::make("123|.0.0.0"));
    QCOMPARE(actual.count(), 1);
    QCOMPARE((actual[0].start), (State{"123.0.0.0", 3}));
    QCOMPARE((actual[0].expected), (State{"123.0.0.0", 3}));
    QCOMPARE((actual[0].finish_value), ("123.0.0.0"));
}

void Test_OneClickHelpers::test_make_moveonly_cases_insert()
{
    auto test_factory = OnlyPosMove(Click('.'));
    auto actual = test_factory.make(ClickEffect::InputSet::make("123|.0.0.0"));
    QCOMPARE(actual.count(), 1);
    QCOMPARE((actual[0].start), (State{"123.0.0.0", 3}));
    QCOMPARE((actual[0].expected), (State{"123.0.0.0", 4}));
    QCOMPARE((actual[0].finish_value), ("123.0.0.0"));
}

void Test_OneClickHelpers::test_make_moveonly_cases_remove_forward()
{
    auto test_factory = OnlyPosMove(Click(Qt::Key_Delete));
    auto actual = test_factory.make(ClickEffect::InputSet::make("123|.0.0.0"));
    QCOMPARE(actual.count(), 1);
    QCOMPARE((actual[0].start), (State{"123.0.0.0", 3}));
    QCOMPARE((actual[0].expected), (State{"123.0.0.0", 4}));
    QCOMPARE((actual[0].finish_value), ("123.0.0.0"));
}

void Test_OneClickHelpers::test_make_moveonly_cases_remove_backward()
{
    auto test_factory = OnlyPosMove(Click(Qt::Key_Backspace));
    auto actual = test_factory.make(ClickEffect::InputSet::make("123.|0.0.0"));
    QCOMPARE(actual.count(), 1);
    QCOMPARE((actual[0].start), (State{"123.0.0.0", 4}));
    QCOMPARE((actual[0].expected), (State{"123.0.0.0", 3}));
    QCOMPARE((actual[0].finish_value), ("123.0.0.0"));
}

void Test_OneClickHelpers::test_make_insertover_cases()
{
    auto test_factory = InsertOver(Click('1'));
    auto actual = test_factory.make(ClickEffect::InputSet::make("12|3.0.0.0"));
    QCOMPARE(actual.count(), 1);
    QCOMPARE((actual[0].start), (State{"123.0.0.0", 2}));
    QCOMPARE((actual[0].expected), (State{"121.0.0.0", 3}));
    QCOMPARE((actual[0].finish_value), ("121.0.0.0"));
}

void Test_OneClickHelpers::test_make_replacebefore_cases()
{
    auto test_factory = ReplaceBefore(Click('1'));
    auto actual = test_factory.make(ClickEffect::InputSet::make("0.0|.0.0"));
    QCOMPARE(actual.count(), 1);
    QCOMPARE((actual[0].start), (State{"0.0.0.0", 3}));
    QCOMPARE((actual[0].expected), (State{"0.1.0.0", 3}));
    QCOMPARE((actual[0].finish_value), ("0.1.0.0"));
}

void Test_OneClickHelpers::test_make_test_name_insert_ch()
{
    const QString expected = "enter `.` to 12|.12.12.12";
    const QString actual = Click('.').make_test_name(state);
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_test_name_insert_key()
{
    const QString expected = "enter `0` to 12|.12.12.12";
    const QString actual = Click(Qt::Key_0).make_test_name(state);
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_test_name_remove_forward()
{
    const QString expected = "backspace in 12|.12.12.12";
    const QString actual = Click(Qt::Key_Backspace).make_test_name(state);
    QCOMPARE(actual, expected);
}

void Test_OneClickHelpers::test_make_test_name_remove_backward()
{
    const QString expected = "delete in 12|.12.12.12";
    const QString actual = Click(Qt::Key_Delete).make_test_name(state);
    QCOMPARE(actual, expected);
}
