#include "test_ip_edit.h"

#include "oneclick_testfactory.h"

Test_IpEdit::Test_IpEdit(QObject *parent) : QObject(parent) {}

void Test_IpEdit::oneClick_data(){
    static const QString zero_ip{"0.0.0.0"};

    OneClickTestFactory test_factory;
    test_factory.make_schema();
    /*
        QTest::newRow("test name")
            << "start_value" << start_pos
            << is_key_char << click_key << 'click_char'
            << "ip_edit_expected_value" << ip_edit_expected_pos
            << "ip_edit_finish_expected_value"

        test_factory.fill_data({
            "backspace in 0|", // name
            Click(Qt::Key_Backspace), // key
            {"0", 1}, // start
            {"", 0}, // expected
            "0.0.0.0" // finish
        });
*/

    // === enter zero ===
    {
        const auto case_factory = OnlyPosMove(Click(Qt::Key_0));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|0.|0.|0.|0")
            ));
    }
    {
        const auto case_factory = Invalid(Click(Qt::Key_0));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("0|.0|.0|.0|")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|1.|1.|1.|1")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|10.|10.|10.|10")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|12.|12.|12.|12")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|1|0|0|.|1|0|0|.|1|0|0|.|1|0|0|")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|1|2|3|.|1|2|3|.|1|2|3|.|1|2|3|")
            ));
    }
    {
        const auto case_factory = Valid(Click(Qt::Key_0));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("1|.1|.1|.1|")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("1|0|.1|0|.1|0|.1|0|")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("1|2|.1|2|.1|2|.1|2|")
            ));
    }

    // === enter non zero digit ===
    {
        const auto case_factory = Valid(Click(Qt::Key_1));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|0.|0.|0.|0")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|2|3|.|2|3|.|2|3|.|2|3|")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|1|0|.|1|0|.|1|0|.|1|0|")
            ));
    }
    {
        const auto case_factory = ReplaceBefore(Click(Qt::Key_1));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("0|.0|.0|.0|")
            ));
    }
    {
        const auto case_factory = Invalid(Click(Qt::Key_1));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|1|0|0|.|1|0|0|.|1|0|0|.|1|0|0|")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|1|2|3|.|1|2|3|.|1|2|3|.|1|2|3|")
            ));
    }

    // === max octet value ===
    {
        const auto case_factory = Valid(Click(Qt::Key_5));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("2|5|.2|5|.2|5|.2|5|")
            ));
    }
    {
        const auto case_factory = Valid(Click(Qt::Key_2));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|55.|55.|55.|55")
            ));
    }
    {
        const auto case_factory =Invalid(Click(Qt::Key_3));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|5|5|.|5|5|.|5|5|.|5|5|")
            ));
    }

    // === special chars after octet ===
    {
        const auto case_factory = OnlyPosMove(Click(Qt::Key_Space));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("0|.0|.0|.0")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("1|.1|.1|.1")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("10|.10|.10|.10")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("12|.12|.12|.12")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("100|.100|.100|.100")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("123|.123|.123|.123")
            ));
    }

    {
        const auto case_factory = Invalid(Click(Qt::Key_Space));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("0.0.0.0|")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("1.1.1.1|")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("10.10.10.10|")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("12.12.12.12|")
            ));
    }

    {
        const auto case_factory = OnlyPosMove(Click('.'));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("0|.0|.0|.0")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("1|.1|.1|.1")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("10|.10|.10|.10")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("12|.12|.12|.12")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("100|.100|.100|.100")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("123|.123|.123|.123")
            ));
    }
    {
        const auto case_factory = OnlyPosMove(Click(','));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("0|.0|.0|.0")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("1|.1|.1|.1")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("10|.10|.10|.10")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("12|.12|.12|.12")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("100|.100|.100|.100")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("123|.123|.123|.123")
            ));
    }

    // === special chars before octet ===
    {
        test_factory.fill_data({
            "enter ` ` in |0.0.0.0",
            Click(Qt::Key_Space),
            {"0.0.0.0", 0},
            {".0.0.0", 1},
            "0.0.0.0"
        });
        test_factory.fill_data({
            "enter ` ` in 0.|0.0.0",
            Click(Qt::Key_Space),
            {"0.0.0.0", 2},
            {"0..0.0", 3},
            "0.0.0.0"
        });
        test_factory.fill_data({
            "enter ` ` in 0.0.|0.0",
            Click(Qt::Key_Space),
            {"0.0.0.0", 4},
            {"0.0..0", 5},
            "0.0.0.0"
        });
        test_factory.fill_data({
            "enter ` ` in 0.0.0.|0",
            Click(Qt::Key_Space),
            {"0.0.0.0", 6},
            {"0.0.0.", 6},
            "0.0.0.0"
        });

        test_factory.fill_data({
            "enter ` ` in |1.1.1.1",
            Click(Qt::Key_Space),
            {"1.1.1.1", 0},
            {".1.1.1", 1},
            "0.1.1.1"
        });
        test_factory.fill_data({
            "enter ` ` in 1.|1.1.1",
            Click(Qt::Key_Space),
            {"1.1.1.1", 2},
            {"1..1.1", 3},
            "1.0.1.1"
        });
        test_factory.fill_data({
            "enter ` ` in 1.1.|1.1",
            Click(Qt::Key_Space),
            {"1.1.1.1", 4},
            {"1.1..1", 5},
            "1.1.0.1"
        });
        test_factory.fill_data({
            "enter ` ` in 1.1.1.|1",
            Click(Qt::Key_Space),
            {"1.1.1.1", 6},
            {"1.1.1.", 6},
            "1.1.1.0"
        });

        test_factory.fill_data({
            "enter ` ` in |10.10.10.10",
            Click(Qt::Key_Space),
            {"10.10.10.10", 0},
            {".10.10.10", 1},
            "0.10.10.10"
        });
        test_factory.fill_data({
            "enter ` ` in 10.|10.10.10",
            Click(Qt::Key_Space),
            {"10.10.10.10", 3},
            {"10..10.10", 4},
            "10.0.10.10"
        });
        test_factory.fill_data({
            "enter ` ` in 10.10.|10.10",
            Click(Qt::Key_Space),
            {"10.10.10.10", 6},
            {"10.10..10", 7},
            "10.10.0.10"
        });
        test_factory.fill_data({
            "enter ` ` in 10.10.10.|10",
            Click(Qt::Key_Space),
            {"10.10.10.10", 9},
            {"10.10.10.", 9},
            "10.10.10.0"
        });

        test_factory.fill_data({
            "enter ` ` in |12.12.12.12",
            Click(Qt::Key_Space),
            {"12.12.12.12", 0},
            {".12.12.12", 1},
            "0.12.12.12"
        });
        test_factory.fill_data({
            "enter ` ` in 12.|12.12.12",
            Click(Qt::Key_Space),
            {"12.12.12.12", 3},
            {"12..12.12", 4},
            "12.0.12.12"
        });
        test_factory.fill_data({
            "enter ` ` in 12.12.|12.12",
            Click(Qt::Key_Space),
            {"12.12.12.12", 6},
            {"12.12..12", 7},
            "12.12.0.12"
        });
        test_factory.fill_data({
            "enter ` ` in 12.12.12.|12",
            Click(Qt::Key_Space),
            {"12.12.12.12", 9},
            {"12.12.12.", 9},
            "12.12.12.0"
        });

        test_factory.fill_data({
            "enter ` ` in |100.100.100.100",
            Click(Qt::Key_Space),
            {"100.100.100.100", 0},
            {".100.100.100", 1},
            "0.100.100.100"
        });
        test_factory.fill_data({
            "enter ` ` in 100.|100.100.100",
            Click(Qt::Key_Space),
            {"100.100.100.100", 4},
            {"100..100.100", 5},
            "100.0.100.100"
        });
        test_factory.fill_data({
            "enter ` ` in 100.100.|100.100",
            Click(Qt::Key_Space),
            {"100.100.100.100", 8},
            {"100.100..100", 9},
            "100.100.0.100"
        });
        test_factory.fill_data({
            "enter ` ` in 100.100.100.|100",
            Click(Qt::Key_Space),
            {"100.100.100.100", 12},
            {"100.100.100.", 12},
            "100.100.100.0"
        });

        test_factory.fill_data({
            "enter ` ` in |101.101.101.101",
            Click(Qt::Key_Space),
            {"101.101.101.101", 0},
            {".101.101.101", 1},
            "0.101.101.101"
        });
        test_factory.fill_data({
            "enter ` ` in 101.|101.101.101",
            Click(Qt::Key_Space),
            {"101.101.101.101", 4},
            {"101..101.101", 5},
            "101.0.101.101"
        });
        test_factory.fill_data({
            "enter ` ` in 101.101.|101.101",
            Click(Qt::Key_Space),
            {"101.101.101.101", 8},
            {"101.101..101", 9},
            "101.101.0.101"
        });
        test_factory.fill_data({
            "enter ` ` in 101.101.101.|101",
            Click(Qt::Key_Space),
            {"101.101.101.101", 12},
            {"101.101.101.", 12},
            "101.101.101.0"
        });

        test_factory.fill_data({
            "enter ` ` in |120.120.120.120",
            Click(Qt::Key_Space),
            {"120.120.120.120", 0},
            {".120.120.120", 1},
            "0.120.120.120"
        });
        test_factory.fill_data({
            "enter ` ` in 120.|120.120.120",
            Click(Qt::Key_Space),
            {"120.120.120.120", 4},
            {"120..120.120", 5},
            "120.0.120.120"
        });
        test_factory.fill_data({
            "enter ` ` in 120.120.|120.120",
            Click(Qt::Key_Space),
            {"120.120.120.120", 8},
            {"120.120..120", 9},
            "120.120.0.120"
        });
        test_factory.fill_data({
            "enter ` ` in 120.120.120.|120",
            Click(Qt::Key_Space),
            {"120.120.120.120", 12},
            {"120.120.120.", 12},
            "120.120.120.0"
        });

        test_factory.fill_data({
            "enter ` ` in |123.123.123.123",
            Click(Qt::Key_Space),
            {"123.123.123.123", 0},
            {".123.123.123", 1},
            "0.123.123.123"
        });
        test_factory.fill_data({
            "enter ` ` in 123.|123.123.123",
            Click(Qt::Key_Space),
            {"123.123.123.123", 4},
            {"123..123.123", 5},
            "123.0.123.123"
        });
        test_factory.fill_data({
            "enter ` ` in 123.123.|123.123",
            Click(Qt::Key_Space),
            {"123.123.123.123", 8},
            {"123.123..123", 9},
            "123.123.0.123"
        });
        test_factory.fill_data({
            "enter ` ` in 123.123.123.|123",
            Click(Qt::Key_Space),
            {"123.123.123.123", 12},
            {"123.123.123.", 12},
            "123.123.123.0"
        });
    }

    // === special chars between digits ===
    {
        test_factory.fill_data({
            "enter ` ` to 1|0.10.10.10",
            Click(Qt::Key_Space),
            {"10.10.10.10", 1},
            {"1.10.10.10", 2},
            "1.10.10.10"
        });
        test_factory.fill_data({
            "enter ` ` to 10.1|0.10.10",
            Click(Qt::Key_Space),
            {"10.10.10.10", 4},
            {"10.1.10.10", 5},
            "10.1.10.10"
        });
        test_factory.fill_data({
            "enter ` ` to 10.10.1|0.10",
            Click(Qt::Key_Space),
            {"10.10.10.10", 7},
            {"10.10.1.10", 8},
            "10.10.1.10"
        });
        test_factory.fill_data({
            "enter ` ` to 10.10.10.1|0",
            Click(Qt::Key_Space),
            {"10.10.10.10", 10},
            {"10.10.10.1", 10},
            "10.10.10.1"
        });

        test_factory.fill_data({
            "enter ` ` to 1|2.12.12.12",
            Click(Qt::Key_Space),
            {"12.12.12.12", 1},
            {"1.12.12.12", 2},
            "1.12.12.12"
        });
        test_factory.fill_data({
            "enter ` ` to 12.1|2.12.12",
            Click(Qt::Key_Space),
            {"12.12.12.12", 4},
            {"12.1.12.12", 5},
            "12.1.12.12"
        });
        test_factory.fill_data({
            "enter ` ` to 12.12.1|2.12",
            Click(Qt::Key_Space),
            {"12.12.12.12", 7},
            {"12.12.1.12", 8},
            "12.12.1.12"
        });
        test_factory.fill_data({
            "enter ` ` to 12.12.12.1|2",
            Click(Qt::Key_Space),
            {"12.12.12.12", 10},
            {"12.12.12.1", 10},
            "12.12.12.1"
        });

        test_factory.fill_data({
            "enter ` ` to 1|00.100.100.100",
            Click(Qt::Key_Space),
            {"100.100.100.100", 1},
            {"1.100.100.100", 2},
            "1.100.100.100"
        });
        test_factory.fill_data({
            "enter ` ` to 100.1|00.100.100",
            Click(Qt::Key_Space),
            {"100.100.100.100", 5},
            {"100.1.100.100", 6},
            "100.1.100.100"
        });
        test_factory.fill_data({
            "enter ` ` to 100.100.1|00.100",
            Click(Qt::Key_Space),
            {"100.100.100.100", 9},
            {"100.100.1.100", 10},
            "100.100.1.100"
        });
        test_factory.fill_data({
            "enter ` ` to 100.100.100.1|00",
            Click(Qt::Key_Space),
            {"100.100.100.100", 13},
            {"100.100.100.1", 13},
            "100.100.100.1"
        });

        test_factory.fill_data({
            "enter ` ` to 1|03.103.103.103",
            Click(Qt::Key_Space),
            {"103.103.103.103", 1},
            {"1.103.103.103", 2},
            "1.103.103.103"
        });
        test_factory.fill_data({
            "enter ` ` to 103.1|03.103.103",
            Click(Qt::Key_Space),
            {"103.103.103.103", 5},
            {"103.1.103.103", 6},
            "103.1.103.103"
        });
        test_factory.fill_data({
            "enter ` ` to 103.103.1|03.103",
            Click(Qt::Key_Space),
            {"103.103.103.103", 9},
            {"103.103.1.103", 10},
            "103.103.1.103"
        });
        test_factory.fill_data({
            "enter ` ` to 103.103.103.1|03",
            Click(Qt::Key_Space),
            {"103.103.103.103", 13},
            {"103.103.103.1", 13},
            "103.103.103.1"
        });

        test_factory.fill_data({
            "enter ` ` to 1|20.120.120.120",
            Click(Qt::Key_Space),
            {"120.120.120.120", 1},
            {"1.120.120.120", 2},
            "1.120.120.120"
        });
        test_factory.fill_data({
            "enter ` ` to 120.1|20.120.120",
            Click(Qt::Key_Space),
            {"120.120.120.120", 5},
            {"120.1.120.120", 6},
            "120.1.120.120"
        });
        test_factory.fill_data({
            "enter ` ` to 120.120.1|20.120",
            Click(Qt::Key_Space),
            {"120.120.120.120", 9},
            {"120.120.1.120", 10},
            "120.120.1.120"
        });
        test_factory.fill_data({
            "enter ` ` to 120.120.120.1|20",
            Click(Qt::Key_Space),
            {"120.120.120.120", 13},
            {"120.120.120.1", 13},
            "120.120.120.1"
        });

        test_factory.fill_data({
            "enter ` ` to 1|23.123.123.123",
            Click(Qt::Key_Space),
            {"123.123.123.123", 1},
            {"1.123.123.123", 2},
            "1.123.123.123"
        });
        test_factory.fill_data({
            "enter ` ` to 123.1|23.123.123",
            Click(Qt::Key_Space),
            {"123.123.123.123", 5},
            {"123.1.123.123", 6},
            "123.1.123.123"
        });
        test_factory.fill_data({
            "enter ` ` to 123.123.1|23.123",
            Click(Qt::Key_Space),
            {"123.123.123.123", 9},
            {"123.123.1.123", 10},
            "123.123.1.123"
        });
        test_factory.fill_data({
            "enter ` ` to 123.123.123.1|23",
            Click(Qt::Key_Space),
            {"123.123.123.123", 13},
            {"123.123.123.1", 13},
            "123.123.123.1"
        });

        test_factory.fill_data({
            "enter ` ` to 10|0.100.100.100",
            Click(Qt::Key_Space),
            {"100.100.100.100", 2},
            {"10.100.100.100", 3},
            "10.100.100.100"
        });
        test_factory.fill_data({
            "enter ` ` to 100.10|0.100.100",
            Click(Qt::Key_Space),
            {"100.100.100.100", 6},
            {"100.10.100.100", 7},
            "100.10.100.100"
        });
        test_factory.fill_data({
            "enter ` ` to 100.100.10|0.100",
            Click(Qt::Key_Space),
            {"100.100.100.100", 10},
            {"100.100.10.100", 11},
            "100.100.10.100"
        });
        test_factory.fill_data({
            "enter ` ` to 100.100.100.10|0",
            Click(Qt::Key_Space),
            {"100.100.100.100", 14},
            {"100.100.100.10", 14},
            "100.100.100.10"
        });

        test_factory.fill_data({
            "enter ` ` to 10|3.103.103.103",
            Click(Qt::Key_Space),
            {"103.103.103.103", 2},
            {"10.103.103.103", 3},
            "10.103.103.103"
        });
        test_factory.fill_data({
            "enter ` ` to 103.10|3.103.103",
            Click(Qt::Key_Space),
            {"103.103.103.103", 6},
            {"103.10.103.103", 7},
            "103.10.103.103"
        });
        test_factory.fill_data({
            "enter ` ` to 103.103.10|3.103",
            Click(Qt::Key_Space),
            {"103.103.103.103", 10},
            {"103.103.10.103", 11},
            "103.103.10.103"
        });
        test_factory.fill_data({
            "enter ` ` to 103.103.103.10|3",
            Click(Qt::Key_Space),
            {"103.103.103.103", 14},
            {"103.103.103.10", 14},
            "103.103.103.10"
        });

        test_factory.fill_data({
            "enter ` ` to 12|0.120.120.120",
            Click(Qt::Key_Space),
            {"120.120.120.120", 2},
            {"12.120.120.120", 3},
            "12.120.120.120"
        });
        test_factory.fill_data({
            "enter ` ` to 120.12|0.120.120",
            Click(Qt::Key_Space),
            {"120.120.120.120", 6},
            {"120.12.120.120", 7},
            "120.12.120.120"
        });
        test_factory.fill_data({
            "enter ` ` to 120.120.12|0.120",
            Click(Qt::Key_Space),
            {"120.120.120.120", 10},
            {"120.120.12.120", 11},
            "120.120.12.120"
        });
        test_factory.fill_data({
            "enter ` ` to 120.120.120.12|0",
            Click(Qt::Key_Space),
            {"120.120.120.120", 14},
            {"120.120.120.12", 14},
            "120.120.120.12"
        });

        test_factory.fill_data({
            "enter ` ` to 12|3.123.123.123",
            Click(Qt::Key_Space),
            {"123.123.123.123", 2},
            {"12.123.123.123", 3},
            "12.123.123.123"
        });
        test_factory.fill_data({
            "enter ` ` to 123.12|3.123.123",
            Click(Qt::Key_Space),
            {"123.123.123.123", 6},
            {"123.12.123.123", 7},
            "123.12.123.123"
        });
        test_factory.fill_data({
            "enter ` ` to 123.123.12|3.123",
            Click(Qt::Key_Space),
            {"123.123.123.123", 10},
            {"123.123.12.123", 11},
            "123.123.12.123"
        });
        test_factory.fill_data({
            "enter ` ` to 123.123.123.12|3",
            Click(Qt::Key_Space),
            {"123.123.123.123", 14},
            {"123.123.123.12", 14},
            "123.123.123.12"
        });
    }

    // === rermove dot ===
    {
        const auto case_factory = OnlyPosMove(Click(Qt::Key_Delete));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("0|.0|.0|.0")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("1|.1|.1|.1")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("10|.10|.10|.10")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("12|.12|.12|.12")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("100|.100|.100|.100")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("120|.120|.120|.120")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("102|.102|.102|.102")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("123|.123|.123|.123")
        ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|.|.|.0")
        ), &zero_ip);

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|.|.0|.")
        ), &zero_ip);

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|.0|.|.")
        ), &zero_ip);

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("0|.|.|.")
        ), &zero_ip);
    }
    {
        const auto case_factory = OnlyPosMove(Click(Qt::Key_Backspace));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("0.|0.|0.|0")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("1.|1.|1.|1")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("10.|10.|10.|10")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("12.|12.|12.|12")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("100.|100.|100.|100")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("120.|120.|120.|120")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("102.|102.|102.|102")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("123.|123.|123.|123")
            ));

        test_factory.TestDataFactory::fill_data(case_factory.make(
                                                    ClickEffect::InputSet::make(".|.|.|0")
                                                    ), &zero_ip);

        test_factory.TestDataFactory::fill_data(case_factory.make(
                                                    ClickEffect::InputSet::make(".|.|0.|")
                                                    ), &zero_ip);

        test_factory.TestDataFactory::fill_data(case_factory.make(
                                                    ClickEffect::InputSet::make(".|0.|.|")
                                                    ), &zero_ip);

        test_factory.TestDataFactory::fill_data(case_factory.make(
                                                    ClickEffect::InputSet::make("0.|.|.|")
                                                    ), &zero_ip);
    }

    // === rermove zero octet ===
    {
        const auto case_factory = Valid(Click(Qt::Key_Backspace));
        test_factory.TestDataFactory::fill_data(case_factory.make(
                                                    ClickEffect::InputSet::make("0|.0|.0|.0|")
                                                    ), &zero_ip);
    }

    {
        const auto case_factory = Valid(Click(Qt::Key_Delete));
        test_factory.TestDataFactory::fill_data(case_factory.make(
                                                    ClickEffect::InputSet::make("|0.|0.|0.|0")
                                                    ), &zero_ip);
    }

    // === remove one digit octet ===
    {
        test_factory.fill_data({
            "delete in to |1.1.1.1",
            Click(Qt::Key_Delete),
            {"1.1.1.1", 0},
            {".1.1.1", 0},
            "0.1.1.1"
        });
        test_factory.fill_data({
            "delete in to 1.|1.1.1",
            Click(Qt::Key_Delete),
            {"1.1.1.1", 2},
            {"1..1.1", 2},
            "1.0.1.1"
        });
        test_factory.fill_data({
            "delete in to 1.1.|1.1",
            Click(Qt::Key_Delete),
            {"1.1.1.1", 4},
            {"1.1..1", 4},
            "1.1.0.1"
        });
        test_factory.fill_data({
            "delete in to 1.1.1.|1",
            Click(Qt::Key_Delete),
            {"1.1.1.1", 6},
            {"1.1.1.", 6},
            "1.1.1.0"
        });

        test_factory.fill_data({
            "backspace in to 1|.1.1.1",
            Click(Qt::Key_Backspace),
            {"1.1.1.1", 1},
            {".1.1.1", 0},
            "0.1.1.1"
        });
        test_factory.fill_data({
            "backspace in to 1.1|.1.1",
            Click(Qt::Key_Backspace),
            {"1.1.1.1", 3},
            {"1..1.1", 2},
            "1.0.1.1"
        });
        test_factory.fill_data({
            "backspace in to 1.1.1|.1",
            Click(Qt::Key_Backspace),
            {"1.1.1.1", 5},
            {"1.1..1", 4},
            "1.1.0.1"
        });
        test_factory.fill_data({
            "backspace in 1.1.1.1|",
            Click(Qt::Key_Backspace),
            {"1.1.1.1", 7},
            {"1.1.1.", 6},
            "1.1.1.0"
        });
    }

    // === remove in two digit octet ===
    {
        const auto case_factory = Valid(Click(Qt::Key_Delete));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|1|2.|1|2.|1|2.|1|2")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|1|0.|1|0.|1|0.|1|0")
            ));
    }
    {
        const auto case_factory = Valid(Click(Qt::Key_Backspace));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("1|2|.1|2|.1|2|.1|2|")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("1|0|.1|0|.1|0|.1|0|")
            ));
    }

    // === remove in three digit octet ===
    {
        const auto case_factory = Valid(Click(Qt::Key_Delete));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|1|2|3.|1|2|3.|1|2|3.|1|2|3")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|1|2|0.|1|2|0.|1|2|0.|1|2|0")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("1|0|3.1|0|3.1|0|3.1|0|3")
            ));
        test_factory.fill_data({
            "delete in |103.103.103.103",
            Click(Qt::Key_Delete),
            {"103.103.103.103", 0},
            {"3.103.103.103", 0},
            "3.103.103.103"
        });
        test_factory.fill_data({
            "delete in 103.|103.103.103",
            Click(Qt::Key_Delete),
            {"103.103.103.103", 4},
            {"103.3.103.103", 4},
            "103.3.103.103"
        });
        test_factory.fill_data({
            "delete in 103.103.|103.103",
            Click(Qt::Key_Delete),
            {"103.103.103.103", 8},
            {"103.103.3.103", 8},
            "103.103.3.103"
        });
        test_factory.fill_data({
            "delete in 103.103.103.|103",
            Click(Qt::Key_Delete),
            {"103.103.103.103", 12},
            {"103.103.103.3", 12},
            "103.103.103.3"
        });
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("1|0|0.1|0|0.1|0|0.1|0|0")
            ));
        test_factory.fill_data({
            "delete in |100.100.100.100",
            Click(Qt::Key_Delete),
            {"100.100.100.100", 0},
            {"0.100.100.100", 0},
            "0.100.100.100"
        });
        test_factory.fill_data({
            "delete in 100.|100.100.100",
            Click(Qt::Key_Delete),
            {"100.100.100.100", 4},
            {"100.0.100.100", 4},
            "100.0.100.100"
        });
        test_factory.fill_data({
            "delete in 100.100.|100.100",
            Click(Qt::Key_Delete),
            {"100.100.100.100", 8},
            {"100.100.0.100", 8},
            "100.100.0.100"
        });
        test_factory.fill_data({
            "delete in 100.100.100.|100",
            Click(Qt::Key_Delete),
            {"100.100.100.100", 12},
            {"100.100.100.0", 12},
            "100.100.100.0"
        });
    }
    {
        const auto case_factory = Valid(Click(Qt::Key_Backspace));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("1|2|3|.1|2|3|.1|2|3|.1|2|3|")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("1|2|0|.1|2|0|.1|2|0|.1|2|0|")
            ));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("10|3|.10|3|.10|3|.10|3|")
            ));
        test_factory.fill_data({
            "backspace in 1|03.103.103.103",
            Click(Qt::Key_Backspace),
            {"103.103.103.103", 1},
            {"3.103.103.103", 0},
            "3.103.103.103"
        });
        test_factory.fill_data({
            "backspace in 103.1|03.103.103",
            Click(Qt::Key_Backspace),
            {"103.103.103.103", 5},
            {"103.3.103.103", 4},
            "103.3.103.103"
        });
        test_factory.fill_data({
            "backspace in 103.103.1|03.103",
            Click(Qt::Key_Backspace),
            {"103.103.103.103", 9},
            {"103.103.3.103", 8},
            "103.103.3.103"
        });
        test_factory.fill_data({
            "backspace in 103.103.103.1|03",
            Click(Qt::Key_Backspace),
            {"103.103.103.103", 13},
            {"103.103.103.3", 12},
            "103.103.103.3"
        });
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("10|0|.10|0|.10|0|.10|0|")
            ));
        test_factory.fill_data({
            "backspace in 1|00.100.100.100",
            Click(Qt::Key_Backspace),
            {"100.100.100.100", 1},
            {"0.100.100.100", 0},
            "0.100.100.100"
        });
        test_factory.fill_data({
            "backspace in 100.1|00.100.100",
            Click(Qt::Key_Backspace),
            {"100.100.100.100", 5},
            {"100.0.100.100", 4},
            "100.0.100.100"
        });
        test_factory.fill_data({
            "backspace in 100.100.1|00.100",
            Click(Qt::Key_Backspace),
            {"100.100.100.100", 9},
            {"100.100.0.100", 8},
            "100.100.0.100"
        });
        test_factory.fill_data({
            "backspace in 100.100.100.1|00",
            Click(Qt::Key_Backspace),
            {"100.100.100.100", 13},
            {"100.100.100.0", 12},
            "100.100.100.0"
        });
    }

    // === remove in int value ===
    {
        test_factory.fill_data({
            "delete in |4294967295",
            Click(Qt::Key_Delete),
            {"4294967295", 0},
            {"294967295", 0},
            "17.148.215.255"
        });

        test_factory.fill_data({
            "delete in 4294|967295",
            Click(Qt::Key_Delete),
            {"4294967295", 4},
            {"429467295", 4},
            "25.153.38.159"
        });

        test_factory.fill_data({
            "delete in 429496729|5",
            Click(Qt::Key_Delete),
            {"4294967295", 9},
            {"429496729", 9},
            "25.153.153.153"
        });
    }
    {
        test_factory.fill_data({
            "backspace in 4|294967295",
            Click(Qt::Key_Backspace),
            {"4294967295", 1},
            {"294967295", 0},
            "17.148.215.255"
        });

        test_factory.fill_data({
            "backspace in 42949|67295",
            Click(Qt::Key_Backspace),
            {"4294967295", 5},
            {"429467295", 4},
            "25.153.38.159"
        });

        test_factory.fill_data({
            "backspace in 4294967295|",
            Click(Qt::Key_Backspace),
            {"4294967295", 10},
            {"429496729", 9},
            "25.153.153.153"
        });
    }

    // === enter zero to int value ===
    {
        test_factory.fill_data({
            "enter `0` to |1",
            Click(Qt::Key_0),
            {"1", 0},
            {"1", 0},
            "0.0.0.1"
        });

        test_factory.fill_data({
            "enter `0` to |99",
            Click(Qt::Key_0),
            {"99", 0},
            {"99", 0},
            "0.0.0.99"
        });

        test_factory.fill_data({
            "enter `0` to 9|9",
            Click(Qt::Key_0),
            {"99", 1},
            {"909", 2},
            "0.0.3.141"
        });

        test_factory.fill_data({
            "enter `0` to 99|",
            Click(Qt::Key_0),
            {"99", 2},
            {"990", 3},
            "0.0.3.222"
        });
    }

    // === enter non zero to int value ===
    {
        const QString finish_val{"0.0.3.231"};

        const auto case_factory = Valid(Click(Qt::Key_9));
        test_factory.TestDataFactory::fill_data(case_factory.make(
            ClickEffect::InputSet::make("|9|9|")), &finish_val);
    }

    // === enter special char to int value ===
    {
        test_factory.fill_data({
            "enter `.` to |99",
            Click('.'),
            {"99", 0},
            {".99..", 1},
            "0.99.0.0"
        });

        test_factory.fill_data({
            "enter `.` to 9|9",
            Click('.'),
            {"99", 1},
            {"9.9..", 2},
            "9.9.0.0"
        });

        test_factory.fill_data({
            "enter `.` to 99|",
            Click('.'),
            {"99", 2},
            {"99...", 3},
            "99.0.0.0"
        });
    }
    {
        const QString calculated_1111111111 = "66.58.53.199";
        const auto case_factory = Invalid(Click('.'));
        test_factory.TestDataFactory::fill_data(case_factory.make(
                                                    ClickEffect::InputSet::make("|1111|1|1|1|1|1|1|")
                                                    ), &calculated_1111111111);
    }
    {
        const QString calculated_1111111111 = "66.58.53.199";
        const auto case_factory = Invalid(Click(' '));
        test_factory.TestDataFactory::fill_data(case_factory.make(
                                                    ClickEffect::InputSet::make("|1111|1|1|1|1|1|1|")
                                                    ), &calculated_1111111111);
    }
    {
        test_factory.fill_data({
            "enter `.` in 1|111111111",
            Click('.'),
            {"1111111111", 1},
            {"1.111.111.111", 2},
            "1.111.111.111"
        });

        test_factory.fill_data({
            "enter `.` in 11|11111111",
            Click('.'),
            {"1111111111", 2},
            {"11.111.111.11", 3},
            "11.111.111.11"
        });

        test_factory.fill_data({
            "enter `.` in 111|1111111",
            Click('.'),
            {"1111111111", 3},
            {"111.111.111.1", 4},
            "111.111.111.1"
        });

        test_factory.fill_data({
            "enter ` ` in 1|111111111",
            Click(' '),
            {"1111111111", 1},
            {"1.111.111.111", 2},
            "1.111.111.111"
        });

        test_factory.fill_data({
            "enter ` ` in 11|11111111",
            Click(' '),
            {"1111111111", 2},
            {"11.111.111.11", 3},
            "11.111.111.11"
        });

        test_factory.fill_data({
            "enter ` ` in 111|1111111",
            Click(' '),
            {"1111111111", 3},
            {"111.111.111.1", 4},
            "111.111.111.1"
        });
    }

    {
        test_factory.fill_data({
            "enter ` ` in 255|255",
            Click(' '),
            {"255255", 3},
            {"255.255..", 4},
            "255.255.0.0"
        });

        test_factory.fill_data({
            "enter ` ` in 25|5255",
            Click(' '),
            {"255255", 2},
            {"25.52.55.", 3},
            "25.52.55.0"
        });

        test_factory.fill_data({
            "enter ` ` in 25|1111",
            Click(' '),
            {"251111", 2},
            {"25.111.1.", 3},
            "25.111.1.0"
        });
    }

    // === enter max int value ===
    {
        // max_int = 4294967295
        const QString max_ip = "255.255.255.255";

        const auto case_factory = Valid(Click(Qt::Key_4));
        test_factory.TestDataFactory::fill_data(case_factory.make(
                                                    ClickEffect::InputSet::make("|294967295")
                                                    ), &max_ip);
        test_factory.TestDataFactory::fill_data(case_factory.make(
                                                    ClickEffect::InputSet::make("429|967295")
                                                    ), &max_ip);
    }
    {
        const QString max_ip = "255.255.255.255";

        const auto case_factory = Valid(Click(Qt::Key_5));
        test_factory.TestDataFactory::fill_data(case_factory.make(
                                                    ClickEffect::InputSet::make("429496729|")
                                                    ), &max_ip);
    }
    {
        const QString max_int = "4294967295";
        const QString max_ip = "255.255.255.255";

        const auto case_factory = Valid(Click(Qt::Key_9));
        test_factory.TestDataFactory::fill_data(case_factory.make(
                                                    ClickEffect::InputSet::make("42|4967295")
                                                    ), &max_ip);
        test_factory.TestDataFactory::fill_data(case_factory.make(
                                                    ClickEffect::InputSet::make("4294|67295")
                                                    ), &max_ip);
        test_factory.TestDataFactory::fill_data(case_factory.make(
                                                    ClickEffect::InputSet::make("42949672|5")
                                                    ), &max_ip);
    }

    // === enter overflowed int value ===
    {
        const QString max_ip = "255.255.255.255";

        const auto case_factory = Invalid(Click(Qt::Key_0));
        test_factory.TestDataFactory::fill_data(case_factory.make(
                                                    ClickEffect::InputSet::make("|4|2|9|4|9|6|7|2|9|5|")
                                                    ), &max_ip);
    }
    {
        test_factory.fill_data({
            "enter `5` in |294967295",
            Click(Qt::Key_5),
            {"294967295", 0},
            {"294967295", 0},
            "17.148.215.255"
        });

        test_factory.fill_data({
            "enter `5` in 429|967295",
            Click(Qt::Key_5),
            {"429967295", 3},
            {"429967295", 3},
            "25.160.199.191"
        });

        test_factory.fill_data({
            "enter `6` in 429496729|",
            Click(Qt::Key_6),
            {"429496729", 9},
            {"429496729", 9},
            "25.153.153.153"
        });
    }

    // ----

    test_factory.fill_data({
        "enter `.` in 1111|111111",
        Click('.'),
        {"1111111111", 4},
        {"1111111111", 4},
        "66.58.53.199"
    });

    test_factory.fill_data({
        "enter `.` |",
        Click('.'),
        {"", 0},
        {"...", 1},
        "0.0.0.0"
    });

    test_factory.fill_data({
        "enter `.` 789|",
        Click('.'),
        {"789", 3},
        {"789", 3},
        "0.0.3.21"
    });

    test_factory.fill_data({
        "enter `.` 7|89",
        Click('.'),
        {"789", 1},
        {"7.89..", 2},
        "7.89.0.0"
    });
}

void Test_IpEdit::oneClick(){
    QFETCH(const QString, start_value);
    QFETCH(const int, start_pos);
    QFETCH(const QString, ip_edit_expected_value);
    QFETCH(const QString, ip_edit_finish_expected_value);
    QFETCH(const int, ip_edit_expected_pos);
    QFETCH(const bool, is_key_char);
    QFETCH(const Qt::Key, click_key);
    QFETCH(const char, click_char);


    // For debug reason only (to set brakpoint)
        if (
            start_value == "1.1.1.1"
            &&
            start_pos == 0
            &&
            click_key == Qt::Key_0
            &&
            is_key_char == false
        )
        {
            int a = 1;
            ++a;
        }


    auto ip_edit = std::make_unique<IpEditTestHelper>(start_value);
    ip_edit->start_edit(start_pos);

    // For debug reason only
        auto text_before = ip_edit->text();
        auto pos_before = ip_edit->cursorPosition();

    if (is_key_char)
        ip_edit->click(click_char);
    else
        ip_edit->click(click_key);
    ip_edit->check(ip_edit_expected_value, ip_edit_expected_pos);

    // For debug reason only
        auto text_after = ip_edit->text();
        auto pos_after = ip_edit->cursorPosition();

    ip_edit->finish_edit();
    ip_edit->check(ip_edit_finish_expected_value);
}

void Test_IpEdit::clear_all_by_backspace_0_0_0_0()
{
    auto ip_edit = std::make_unique<IpEditTestHelper>("0.0.0.0");
    ip_edit->start_edit();          // 0.0.0.0|
    ip_edit->check("0.0.0.0", 7);
    ip_edit->click(backspace_key);  // 0.0.0.|
    ip_edit->check("0.0.0.", 6);
    ip_edit->click(backspace_key);  // 0.0.0|.
    ip_edit->check("0.0.0.", 5);
    ip_edit->click(backspace_key);  // 0.0.|.
    ip_edit->check("0.0..", 4);
    ip_edit->click(backspace_key);  // 0.0|..
    ip_edit->check("0.0..", 3);
    ip_edit->click(backspace_key);  // 0.|..
    ip_edit->check("0...", 2);
    ip_edit->click(backspace_key);  // 0|...
    ip_edit->check("0...", 1);
    ip_edit->click(backspace_key);  // |
    ip_edit->check("", 0);
    ip_edit->finish_edit();         // 0.0.0.0
    ip_edit->check("0.0.0.0");
}

void Test_IpEdit::clear_all_by_delete_0_0_0_0()
{
    auto ip_edit = std::make_unique<IpEditTestHelper>("0.0.0.0");
    ip_edit->start_edit(0);         // |0.0.0.0
    ip_edit->check("0.0.0.0", 0);
    ip_edit->click(delete_key);  // |.0.0.0
    ip_edit->check(".0.0.0", 0);
    ip_edit->click(delete_key);  // .|0.0.0
    ip_edit->check(".0.0.0", 1);
    ip_edit->click(delete_key);  // .|.0.0
    ip_edit->check("..0.0", 1);
    ip_edit->click(delete_key);  // ..|0.0
    ip_edit->check("..0.0", 2);
    ip_edit->click(delete_key);  // ..|.0
    ip_edit->check("...0", 2);
    ip_edit->click(delete_key);  // ...|0
    ip_edit->check("...0", 3);
    ip_edit->click(delete_key);  // |
    ip_edit->check("", 0);
    ip_edit->finish_edit();      // 0.0.0.0
    ip_edit->check("0.0.0.0");
}
