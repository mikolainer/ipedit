#ifndef TEST_H
#define TEST_H

#include <memory>
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

class LineEditWithIpValidatorTest : public QLineEdit
{ Q_OBJECT
    constexpr static const int typing_delay = 0;
    constexpr static const Qt::KeyboardModifiers kb_modifier = Qt::NoModifier;

public:
    LineEditWithIpValidatorTest(const QString& text, QWidget* parent = nullptr);

    void start_edit(int cursor_pos = CursorPos::invalid);
    void finish_edit();

    void check(const QString& expected_text, int expected_pos) {
        check(expected_text);
        check(expected_pos);
    };
    void check(const QString& expected_text){
        QCOMPARE(text(), expected_text);
    };
    void check(int expected_pos){
        QCOMPARE(cursorPosition(), expected_pos);
    }

    void click(char key){
        QTest::keyClick(this, key, kb_modifier, typing_delay);
    }
    void click(Qt::Key key){
        QTest::keyClick(this, key, kb_modifier, typing_delay);
    }
};

struct State{
    QString value;
    int pos;

    QString toString() const;

    constexpr bool operator==(const State& other) const{
        return other.value == value
               && other.pos == pos;
    }
};

struct Click{
    constexpr static const Qt::Key default_key{Qt::Key_Any};
    constexpr static const char default_char{' '};

    bool is_char;
    Qt::Key click_key;
    char click_char;

    Click(char key)
        : is_char(true)
        , click_key(Click::default_key)
        , click_char(key)
    {}

    Click(Qt::Key key)
        : is_char(false)
        , click_key(key)
        , click_char(char_from_key(key))
    {}

    Click(bool is_char, Qt::Key key, char ch)
        : is_char(is_char)
        , click_key(key)
        , click_char(ch)
    {}

    Click()
        : is_char(false)
        , click_key(Click::default_key)
        , click_char(Click::default_char)
    {}

    constexpr bool operator==(const Click& other) const{
        return other.is_char == is_char
            && other.click_char == click_char
            && other.click_key == click_key;
    }

    static char char_from_key(Qt::Key key)
    {
        char keychar = default_char;
        switch (key) {
        case Qt::Key_0:
            keychar = '0';
            break;
        case Qt::Key_1:
            keychar = '1';
            break;
        case Qt::Key_2:
            keychar = '2';
            break;
        case Qt::Key_3:
            keychar = '3';
            break;
        case Qt::Key_4:
            keychar = '4';
            break;
        case Qt::Key_5:
            keychar = '5';
            break;
        case Qt::Key_6:
            keychar = '6';
            break;
        case Qt::Key_7:
            keychar = '7';
            break;
        case Qt::Key_8:
            keychar = '8';
            break;
        case Qt::Key_9:
            keychar = '9';
            break;
        case Qt::Key_Space:
            keychar = ' ';
            break;
        case Qt::Key_Delete:
            keychar = default_char;
            break;
        case Qt::Key_Backspace:
            keychar = default_char;
            break;
        default:
            Q_ASSERT(false); // invalid key
        }

        return keychar;
    }

    bool is_forward_remove() const {
        return click_key == Qt::Key_Delete;
    }

    bool is_backward_remove() const {
        return click_key == Qt::Key_Backspace;
    }

    QString make_test_name(const State& start_state) const
    {
        bool is_remove = is_backward_remove() || is_forward_remove();

        return QString("%1 %2 %3")
            .arg(
                operation_name(),
                is_remove ? "in" : QString("`%1` to").arg(click_char),
                start_state.toString()
            );
    }

    QString operation_name() const{
        if (is_backward_remove())
            return "backspace";

        if (is_forward_remove())
            return "delete";

        return "enter";
    }
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
    ClickEffect(const Click& key)
        : m_key(key)
    {}

    virtual QList<Case> make(const InputSet& format) const
    {
        QList<Case> result;

        for (auto pos : format.click_pos)
        {
            Case test;
            test.key = m_key;
            test.start = {format.start_value, pos};
            test.expected = new_value(test.start);
            test.finish_value = finish_value(test.expected.value);
            test.name = m_key.make_test_name(test.start);
            result.append(test);
        }

        return result;
    };

protected:
    virtual State new_value(const State& start) const =0;
    virtual QString finish_value(const QString& expected) const =0;

    const Click m_key;
};

class Valid : public ClickEffect
{
public:
    Valid(const Click& key) : ClickEffect(key) {};

protected:
    State new_value(const State& start) const override{
        auto start_copy = start;

        if (m_key.is_backward_remove())
        {
            auto it = start_copy.value.begin() + start.pos -1;
            start_copy.value.erase(it);
            start_copy.pos--;
        }
        else if (m_key.is_forward_remove())
        {
            auto it = start_copy.value.begin() + start.pos;
            start_copy.value.erase(it);
        }
        else
        {
            char keychar = m_key.click_char;
            start_copy.value = start_copy.value.insert(start.pos, keychar);
            start_copy.pos++;
        }

        return start_copy;
    }
    QString finish_value(const QString& expected) const override{
        return expected;
    }
};

class Invalid : public ClickEffect
{
public:
    Invalid(const Click& key) : ClickEffect(key) {};

protected:
    State new_value(const State& start) const override{
        return start;
    }
    QString finish_value(const QString& expected) const override{
        return expected;
    }
};

class OnlyPosMove : public ClickEffect
{
public:
    OnlyPosMove(const Click& key) : ClickEffect(key) {};

protected:
    State new_value(const State& start) const override{
        auto start_copy = start;
        start_copy.pos += m_key.is_backward_remove() ? -1 : +1;
        return start_copy;
    }
    QString finish_value(const QString& expected) const override{
        return expected;
    }
};

class InsertOver : public ClickEffect
{
public:
    InsertOver(const Click& key) : ClickEffect(key) {
        Q_ASSERT(!key.is_backward_remove() && !key.is_forward_remove());
    };

protected:
    State new_value(const State& start) const override{
        auto start_copy = start;
        start_copy.pos += 1;
        *(start_copy.value.begin() + start.pos) = m_key.click_char;
        return start_copy;
    }
    QString finish_value(const QString& expected) const override{
        return expected;
    }
};

class ReplaceBefore : public ClickEffect
{
public:
    ReplaceBefore(const Click& key) : ClickEffect(key) {
        Q_ASSERT(!key.is_backward_remove() && !key.is_forward_remove());
    };

protected:
    State new_value(const State& start) const override{
        Q_ASSERT(start.pos > 0);

        auto start_copy = start;
        *(start_copy.value.begin() + start.pos -1) = m_key.click_char;
        return start_copy;
    }
    QString finish_value(const QString& expected) const override{
        return expected;
    }
};

class TestDataFactory
{
public:
    virtual void fill_data(const QList<Case>& tests, const QString* finish_value = nullptr){
        for (const Case& test : tests)
        {
            if (finish_value)
            {
                auto test_copy = test;
                test_copy.finish_value = *finish_value;
                this->fill_data(test_copy);
            }
            else
            {
                this->fill_data(test);
            }
        }
    };

    virtual void make_schema() const =0;
    virtual void fill_data(const Case& test) const =0;
};

class OneClickTestFactory : public TestDataFactory
{
public:
    OneClickTestFactory() : TestDataFactory() {}
    void make_schema() const override
    {
        QTest::addColumn<const QString>("start_value");
        QTest::addColumn<const int>("start_pos");

        QTest::addColumn<const bool>("is_key_char");
        QTest::addColumn<const Qt::Key>("click_key");
        QTest::addColumn<const char>("click_char");

        QTest::addColumn<const QString>("ip_edit_expected_value");
        QTest::addColumn<const int>("ip_edit_expected_pos");

        QTest::addColumn<const QString>("ip_edit_finish_expected_value");
    }

    void fill_data(const Case& test) const override
    {
        QTest::newRow(test.name.toUtf8().data())
        << test.start.value << test.start.pos // start
        << test.key.is_char << test.key.click_key << test.key.click_char
        << test.expected.value << test.expected.pos // after click
        << test.finish_value; // after finish
    }
};

class Test_OneClickHelpers : public QObject
{ Q_OBJECT

    const State state{"12.12.12.12", 2};
public:
    Test_OneClickHelpers(QObject* parent = nullptr) : QObject(parent) {}

private slots:
    void test_make_click_ch_space()
    {
        char ch = ' ';
        Click actual = Click(ch);
        Click expected = Click{true, Click::default_key, ch};
        QCOMPARE(actual, expected);
    }

    void test_make_click_ch_dot()
    {
        char ch = '.';
        Click actual = Click(ch);
        Click expected = Click{true, Click::default_key, ch};
        QCOMPARE(actual, expected);
    }

    void test_make_click_ch_0()
    {
        char ch = '0';
        Click actual = Click(ch);
        Click expected = Click{true, Click::default_key, ch};
        QCOMPARE(actual, expected);
    }

    void test_make_click_ch_1()
    {
        char ch = '1';
        Click actual = Click(ch);
        Click expected = Click{true, Click::default_key, ch};
        QCOMPARE(actual, expected);
    }

    void test_make_click_ch_2()
    {
        char ch = '2';
        Click actual = Click(ch);
        Click expected = Click{true, Click::default_key, ch};
        QCOMPARE(actual, expected);
    }

    void test_make_click_ch_3()
    {
        char ch = '3';
        Click actual = Click(ch);
        Click expected = Click{true, Click::default_key, ch};
        QCOMPARE(actual, expected);
    }

    void test_make_click_ch_4()
    {
        char ch = '4';
        Click actual = Click(ch);
        Click expected = Click{true, Click::default_key, ch};
        QCOMPARE(actual, expected);
    }

    void test_make_click_ch_5()
    {
        char ch = '5';
        Click actual = Click(ch);
        Click expected = Click{true, Click::default_key, ch};
        QCOMPARE(actual, expected);
    }

    void test_make_click_ch_6()
    {
        char ch = '6';
        Click actual = Click(ch);
        Click expected = Click{true, Click::default_key, ch};
        QCOMPARE(actual, expected);
    }

    void test_make_click_ch_7()
    {
        char ch = '7';
        Click actual = Click(ch);
        Click expected = Click{true, Click::default_key, ch};
        QCOMPARE(actual, expected);
    }

    void test_make_click_ch_8()
    {
        char ch = '8';
        Click actual = Click(ch);
        Click expected = Click{true, Click::default_key, ch};
        QCOMPARE(actual, expected);
    }

    void test_make_click_ch_9()
    {
        char ch = '9';
        Click actual = Click(ch);
        Click expected = Click{true, Click::default_key, ch};
        QCOMPARE(actual, expected);
    }

    void test_make_click_ch_comma()
    {
        char ch = ',';
        Click actual = Click(ch);
        Click expected = Click{true, Click::default_key, ch};
        QCOMPARE(actual, expected);
    }

    void test_make_click_key_space()
    {
        Qt::Key key = Qt::Key_Space;
        Click actual(key);
        Click expected{false, key, Click::default_char};
        QCOMPARE(actual, expected);
    }

    void test_make_click_key_0()
    {
        Qt::Key key = Qt::Key_0;
        Click actual(key);
        Click expected{false, key, '0'};
        QCOMPARE(actual, expected);
    }

    void test_make_click_key_1()
    {
        Qt::Key key = Qt::Key_1;
        Click actual(key);
        Click expected{false, key, '1'};
        QCOMPARE(actual, expected);
    }

    void test_make_click_key_2()
    {
        Qt::Key key = Qt::Key_2;
        Click actual(key);
        Click expected{false, key, '2'};
        QCOMPARE(actual, expected);
    }

    void test_make_click_key_3()
    {
        Qt::Key key = Qt::Key_3;
        Click actual(key);
        Click expected{false, key, '3'};
        QCOMPARE(actual, expected);
    }

    void test_make_click_key_4()
    {
        Qt::Key key = Qt::Key_4;
        Click actual(key);
        Click expected{false, key, '4'};
        QCOMPARE(actual, expected);
    }

    void test_make_click_key_5()
    {
        Qt::Key key = Qt::Key_5;
        Click actual(key);
        Click expected{false, key, '5'};
        QCOMPARE(actual, expected);
    }

    void test_make_click_key_6()
    {
        Qt::Key key = Qt::Key_6;
        Click actual(key);
        Click expected{false, key, '6'};
        QCOMPARE(actual, expected);
    }

    void test_make_click_key_7()
    {
        Qt::Key key = Qt::Key_7;
        Click actual(key);
        Click expected{false, key, '7'};
        QCOMPARE(actual, expected);
    }

    void test_make_click_key_8()
    {
        Qt::Key key = Qt::Key_8;
        Click actual(key);
        Click expected{false, key, '8'};
        QCOMPARE(actual, expected);
    }

    void test_make_click_key_9()
    {
        Qt::Key key = Qt::Key_9;
        Click actual(key);
        Click expected{false, key, '9'};
        QCOMPARE(actual, expected);
    }

    void test_make_input_set()
    {
        auto actual = ClickEffect::InputSet::make("|1.|1|.1|.1|");
        const QString expected_value{"1.1.1.1"};
        const QList<int> expected_pos_set{0,2,3,5,7};

        QCOMPARE(actual.start_value, expected_value);
        QCOMPARE(actual.click_pos, expected_pos_set);
    }

    void test_make_valid_cases_insert()
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

    void test_make_valid_cases_remove_forward()
    {
        auto test_factory = Valid(Click(Qt::Key_Delete));
        auto actual = test_factory.make(ClickEffect::InputSet::make("1|2.12.12.12"));
        QCOMPARE(actual.count(), 1);
        QCOMPARE((actual[0].start), (State{"12.12.12.12", 1}));
        QCOMPARE((actual[0].expected), (State{"1.12.12.12", 1}));
        QCOMPARE((actual[0].finish_value), ("1.12.12.12"));
    }

    void test_make_valid_cases_remove_backward()
    {
        auto test_factory = Valid(Click(Qt::Key_Backspace));
        auto actual = test_factory.make(ClickEffect::InputSet::make("12|.12.12.12"));
        QCOMPARE(actual.count(), 1);
        QCOMPARE((actual[0].start), (State{"12.12.12.12", 2}));
        QCOMPARE((actual[0].expected), (State{"1.12.12.12", 1}));
        QCOMPARE((actual[0].finish_value), ("1.12.12.12"));
    }

    void test_make_invalid_cases_insert()
    {
        auto test_factory = Invalid(Click(Qt::Key_0));
        auto actual = test_factory.make(ClickEffect::InputSet::make("123|.0.0.0"));
        QCOMPARE(actual.count(), 1);
        QCOMPARE((actual[0].start), (State{"123.0.0.0", 3}));
        QCOMPARE((actual[0].expected), (State{"123.0.0.0", 3}));
        QCOMPARE((actual[0].finish_value), ("123.0.0.0"));
    }

    void test_make_moveonly_cases_insert()
    {
        auto test_factory = OnlyPosMove(Click('.'));
        auto actual = test_factory.make(ClickEffect::InputSet::make("123|.0.0.0"));
        QCOMPARE(actual.count(), 1);
        QCOMPARE((actual[0].start), (State{"123.0.0.0", 3}));
        QCOMPARE((actual[0].expected), (State{"123.0.0.0", 4}));
        QCOMPARE((actual[0].finish_value), ("123.0.0.0"));
    }

    void test_make_moveonly_cases_remove_forward()
    {
        auto test_factory = OnlyPosMove(Click(Qt::Key_Delete));
        auto actual = test_factory.make(ClickEffect::InputSet::make("123|.0.0.0"));
        QCOMPARE(actual.count(), 1);
        QCOMPARE((actual[0].start), (State{"123.0.0.0", 3}));
        QCOMPARE((actual[0].expected), (State{"123.0.0.0", 4}));
        QCOMPARE((actual[0].finish_value), ("123.0.0.0"));
    }

    void test_make_moveonly_cases_remove_backward()
    {
        auto test_factory = OnlyPosMove(Click(Qt::Key_Backspace));
        auto actual = test_factory.make(ClickEffect::InputSet::make("123.|0.0.0"));
        QCOMPARE(actual.count(), 1);
        QCOMPARE((actual[0].start), (State{"123.0.0.0", 4}));
        QCOMPARE((actual[0].expected), (State{"123.0.0.0", 3}));
        QCOMPARE((actual[0].finish_value), ("123.0.0.0"));
    }

    void test_make_insertover_cases()
    {
        auto test_factory = InsertOver(Click('1'));
        auto actual = test_factory.make(ClickEffect::InputSet::make("12|3.0.0.0"));
        QCOMPARE(actual.count(), 1);
        QCOMPARE((actual[0].start), (State{"123.0.0.0", 2}));
        QCOMPARE((actual[0].expected), (State{"121.0.0.0", 3}));
        QCOMPARE((actual[0].finish_value), ("121.0.0.0"));
    }

    void test_make_replacebefore_cases()
    {
        auto test_factory = ReplaceBefore(Click('1'));
        auto actual = test_factory.make(ClickEffect::InputSet::make("0.0|.0.0"));
        QCOMPARE(actual.count(), 1);
        QCOMPARE((actual[0].start), (State{"0.0.0.0", 3}));
        QCOMPARE((actual[0].expected), (State{"0.1.0.0", 3}));
        QCOMPARE((actual[0].finish_value), ("0.1.0.0"));
    }

    void test_make_test_name_insert_ch()
    {
        const QString expected = "enter . to 12|.12.12.12";
        const QString actual = Click('.').make_test_name(state);
        QCOMPARE(actual, expected);
    }

    void test_make_test_name_insert_key()
    {
        const QString expected = "enter 0 to 12|.12.12.12";
        const QString actual = Click(Qt::Key_0).make_test_name(state);
        QCOMPARE(actual, expected);
    }

    void test_make_test_name_remove_forward()
    {
        const QString expected = "backspace in 12|.12.12.12";
        const QString actual = Click(Qt::Key_Backspace).make_test_name(state);
        QCOMPARE(actual, expected);
    }

    void test_make_test_name_remove_backward()
    {
        const QString expected = "delete in 12|.12.12.12";
        const QString actual = Click(Qt::Key_Delete).make_test_name(state);
        QCOMPARE(actual, expected);
    }
};

class Test_IpEdit : public QObject
{ Q_OBJECT
    constexpr static const Qt::Key backspace_key = Qt::Key_Backspace;
    constexpr static const Qt::Key delete_key = Qt::Key_Delete;
    constexpr static const Qt::Key rarrow_key = Qt::Key_Right;
    constexpr static const Qt::Key larrow_key = Qt::Key_Left;

public:
    Test_IpEdit(QObject* parent = nullptr) : QObject(parent) {}

private slots:
    void oneClick_data(){
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
                ClickEffect::InputSet::make("|55|.|55.|55.|55")
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
            test_factory.TestDataFactory::fill_data(case_factory.make(
                ClickEffect::InputSet::make("100|.100|.100|.100|")
                ));
            test_factory.TestDataFactory::fill_data(case_factory.make(
                ClickEffect::InputSet::make("123|.123|.123|.123|")
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
                "backspace in to 1|.1.1.1",
                Click(Qt::Key_Backspace),
                {"1.1.1.1", 1},
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
                "backspace in to 1.1|.1.1",
                Click(Qt::Key_Backspace),
                {"1.1.1.1", 3},
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
                "backspace in to 1.1.1|.1",
                Click(Qt::Key_Backspace),
                {"1.1.1.1", 5},
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
                {".100.100.100", 0},
                ".100.100.100"
            });
            test_factory.fill_data({
                "delete in 100.|100.100.100",
                Click(Qt::Key_Delete),
                {"100.100.100.100", 4},
                {"100..100.100", 4},
                "100..100.100"
            });
            test_factory.fill_data({
                "delete in 100.100.|100.100",
                Click(Qt::Key_Delete),
                {"100.100.100.100", 8},
                {"100.100..100", 8},
                "100.100..100"
            });
            test_factory.fill_data({
                "delete in 100.100.100.|100",
                Click(Qt::Key_Delete),
                {"100.100.100.100", 12},
                {"100.100.100.", 12},
                "100.100.100."
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
                {".100.100.100", 0},
                ".100.100.100"
            });
            test_factory.fill_data({
                "backspace in 100.1|00.100.100",
                Click(Qt::Key_Backspace),
                {"100.100.100.100", 5},
                {"100..100.100", 4},
                "100..100.100"
            });
            test_factory.fill_data({
                "backspace in 100.100.1|00.100",
                Click(Qt::Key_Backspace),
                {"100.100.100.100", 9},
                {"100.100..100", 8},
                "100.100..100"
            });
            test_factory.fill_data({
                "backspace in 100.100.100.1|00",
                Click(Qt::Key_Backspace),
                {"100.100.100.100", 13},
                {"100.100.100.", 12},
                "100.100.100."
            });
        }

        // === remove in int value ===
        {

        }

        // === enter non zero to int value ===
        {

        }

        // === enter zero to int value ===
        {

        }

        // === enter special char to int value ===
        {

        }

        // === enter max int value ===
        {

        }

        // === enter overflowed int value ===
        {

        }
    }
    void oneClick(){
        QFETCH(const QString, start_value);
        QFETCH(const int, start_pos);
        QFETCH(const QString, ip_edit_expected_value);
        QFETCH(const QString, ip_edit_finish_expected_value);
        QFETCH(const int, ip_edit_expected_pos);
        QFETCH(const bool, is_key_char);
        QFETCH(const Qt::Key, click_key);
        QFETCH(const char, click_char);

        if (
            start_value == "1.1.1.1"
            &&
            start_pos == 1
            &&
            click_key == delete_key
            &&
            is_key_char == false
        )
        {
            int a = 1;
            ++a;
        }

        auto ip_edit = std::make_unique<LineEditWithIpValidatorTest>(start_value);
        ip_edit->start_edit(start_pos);

        auto text_before = ip_edit->text();
        auto pos_before = ip_edit->cursorPosition();

        if (is_key_char)
            ip_edit->click(click_char);
        else
            ip_edit->click(click_key);
        ip_edit->check(ip_edit_expected_value, ip_edit_expected_pos);

        auto text_after = ip_edit->text();
        auto pos_after = ip_edit->cursorPosition();

        ip_edit->finish_edit();
        ip_edit->check(ip_edit_finish_expected_value);
    };

    void clear_all_by_backspace_0_0_0_0();
    void clear_all_by_delete_0_0_0_0();
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
