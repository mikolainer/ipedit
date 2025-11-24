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
        QTest::addColumn<const QString>("start_value");
        QTest::addColumn<const int>("start_pos");

        QTest::addColumn<const bool>("is_key_char");
        QTest::addColumn<const Qt::Key>("click_key");
        QTest::addColumn<const char>("click_char");

        QTest::addColumn<const QString>("ip_edit_expected_value");
        QTest::addColumn<const int>("ip_edit_expected_pos");

        QTest::addColumn<const QString>("ip_edit_finish_expected_value");

//        QTest::newRow("test name")
//            << "start_value" << start_pos
//            << is_key_char << click_key << 'click_char'
//            << "ip_edit_expected_value" << ip_edit_expected_pos
//            << "ip_edit_finish_expected_value"

        // === enter zero ===
        // enter 0 around zero octets
        QTest::newRow("enter 0 to |0.0.0.0")
            << "0.0.0.0" << 0 // start
            << false << Qt::Key_0 << '0'
            << "0.0.0.0" << 1 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("enter 0 to 0|.0.0.0")
            << "0.0.0.0" << 1 // start
            << false << Qt::Key_0 << '0'
            << "0.0.0.0" << 1 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("enter 0 to 0.|0.0.0")
            << "0.0.0.0" << 2 // start
            << false << Qt::Key_0 << '0'
            << "0.0.0.0" << 3 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("enter 0 to 0.0|.0.0")
            << "0.0.0.0" << 3 // start
            << false << Qt::Key_0 << '0'
            << "0.0.0.0" << 3 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("enter 0 to 0.0.|0.0")
            << "0.0.0.0" << 4 // start
            << false << Qt::Key_0 << '0'
            << "0.0.0.0" << 5 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("enter 0 to 0.0.0|.0")
            << "0.0.0.0" << 5 // start
            << false << Qt::Key_0 << '0'
            << "0.0.0.0" << 5 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("enter 0 to 0.0.0.|0")
            << "0.0.0.0" << 6 // start
            << false << Qt::Key_0 << '0'
            << "0.0.0.0" << 7 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("enter 0 to 0.0.0.0|")
            << "0.0.0.0" << 7 // start
            << false << Qt::Key_0 << '0'
            << "0.0.0.0" << 7 // after click
            << "0.0.0.0"; // after finish

        // enter 0 around one digit octets
        QTest::newRow("enter 0 to |1.1.1.1")
            << "1.1.1.1" << 0 // start
            << false << Qt::Key_0 << '0'
            << "1.1.1.1" << 0 // after click
            << "1.1.1.1"; // after finish

        QTest::newRow("enter 0 to 1|.1.1.1")
            << "1.1.1.1" << 1 // start
            << false << Qt::Key_0 << '0'
            << "10.1.1.1" << 2 // after click
            << "10.1.1.1"; // after finish

        QTest::newRow("enter 0 to 1.|1.1.1")
            << "1.1.1.1" << 2 // start
            << false << Qt::Key_0 << '0'
            << "1.1.1.1" << 2 // after click
            << "1.1.1.1"; // after finish

        QTest::newRow("enter 0 to 1.1|.1.1")
            << "1.1.1.1" << 3 // start
            << false << Qt::Key_0 << '0'
            << "1.10.1.1" << 4 // after click
            << "1.10.1.1"; // after finish

        QTest::newRow("enter 0 to 1.1.|1.1")
            << "1.1.1.1" << 4 // start
            << false << Qt::Key_0 << '0'
            << "1.1.1.1" << 4 // after click
            << "1.1.1.1"; // after finish

        QTest::newRow("enter 0 to 1.1.1|.1")
            << "1.1.1.1" << 5 // start
            << false << Qt::Key_0 << '0'
            << "1.1.10.1" << 6 // after click
            << "1.1.10.1"; // after finish

        QTest::newRow("enter 0 to 1.1.1.|1")
            << "1.1.1.1" << 6 // start
            << false << Qt::Key_0 << '0'
            << "1.1.1.1" << 6 // after click
            << "1.1.1.1"; // after finish

        QTest::newRow("enter 0 to 1.1.1.1|")
            << "1.1.1.1" << 7 // start
            << false << Qt::Key_0 << '0'
            << "1.1.1.10" << 8 // after click
            << "1.1.1.10"; // after finish

        // enter 0 around two digit octet
        QTest::newRow("enter 0 to |12.12.12.12")
            << "12.12.12.12" << 0 // start
            << false << Qt::Key_0 << '0'
            << "12.12.12.12" << 0 // after click
            << "12.12.12.12"; // after finish

        QTest::newRow("enter 0 to 12|.12.12.12")
            << "12.12.12.12" << 2 // start
            << false << Qt::Key_0 << '0'
            << "120.12.12.12" << 3 // after click
            << "120.12.12.12"; // after finish

        QTest::newRow("enter 0 to 12.|12.12.12")
            << "12.12.12.12" << 3 // start
            << false << Qt::Key_0 << '0'
            << "12.12.12.12" << 3 // after click
            << "12.12.12.12"; // after finish

        QTest::newRow("enter 0 to 12.12|.12.12")
            << "12.12.12.12" << 5 // start
            << false << Qt::Key_0 << '0'
            << "12.120.12.12" << 6 // after click
            << "12.120.12.12"; // after finish

        QTest::newRow("enter 0 to 12.12.|12.12")
            << "12.12.12.12" << 6 // start
            << false << Qt::Key_0 << '0'
            << "12.12.12.12" << 6 // after click
            << "12.12.12.12"; // after finish

        QTest::newRow("enter 0 to 12.12.12|.12")
            << "12.12.12.12" << 8 // start
            << false << Qt::Key_0 << '0'
            << "12.12.120.12" << 9 // after click
            << "12.12.120.12"; // after finish

        QTest::newRow("enter 0 to 12.12.12.|12")
            << "12.12.12.12" << 9 // start
            << false << Qt::Key_0 << '0'
            << "12.12.12.12" << 9 // after click
            << "12.12.12.12"; // after finish

        QTest::newRow("enter 0 to 12.12.12.12|")
            << "12.12.12.12" << 11 // start
            << false << Qt::Key_0 << '0'
            << "12.12.12.120" << 12 // after click
            << "12.12.12.120"; // after finish

        // enter 0 to mid of two digit octet
        QTest::newRow("enter 0 to 1|2.12.12.12")
            << "12.12.12.12" << 1 // start
            << false << Qt::Key_0 << '0'
            << "102.12.12.12" << 2 // after click
            << "102.12.12.12"; // after finish

        QTest::newRow("enter 0 to 12.1|2.12.12")
            << "12.12.12.12" << 4 // start
            << false << Qt::Key_0 << '0'
            << "12.102.12.12" << 5 // after click
            << "12.102.12.12"; // after finish

        QTest::newRow("enter 0 to 12.12.1|2.12")
            << "12.12.12.12" << 7 // start
            << false << Qt::Key_0 << '0'
            << "12.12.102.12" << 8 // after click
            << "12.12.102.12"; // after finish

        QTest::newRow("enter 0 to 12.12.12.1|2")
            << "12.12.12.12" << 10 // start
            << false << Qt::Key_0 << '0'
            << "12.12.12.102" << 11 // after click
            << "12.12.12.102"; // after finish

        // enter 0 around three digit octet
        QTest::newRow("enter 0 to |123.123.123.123")
            << "123.123.123.123" << 0 // start
            << false << Qt::Key_0 << '0'
            << "123.123.123.123" << 0 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("enter 0 to 123|.123.123.123")
            << "123.123.123.123" << 3 // start
            << false << Qt::Key_0 << '0'
            << "123.123.123.123" << 3 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("enter 0 to 123.|123.123.123")
            << "123.123.123.123" << 4 // start
            << false << Qt::Key_0 << '0'
            << "123.123.123.123" << 4 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("enter 0 to 123.123|.123.123")
            << "123.123.123.123" << 7 // start
            << false << Qt::Key_0 << '0'
            << "123.123.123.123" << 7 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("enter 0 to 123.123.|123.123")
            << "123.123.123.123" << 8 // start
            << false << Qt::Key_0 << '0'
            << "123.123.123.123" << 8 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("enter 0 to 123.123.123|.123")
            << "123.123.123.123" << 11 // start
            << false << Qt::Key_0 << '0'
            << "123.123.123.123" << 11 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("enter 0 to 123.123.123.|123")
            << "123.123.123.123" << 12 // start
            << false << Qt::Key_0 << '0'
            << "123.123.123.123" << 12 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("enter 0 to 123.123.123.123|")
            << "123.123.123.123" << 15 // start
            << false << Qt::Key_0 << '0'
            << "123.123.123.123" << 15 // after click
            << "123.123.123.123"; // after finish

        // enter 0 to mid of three digit octet
        QTest::newRow("enter 0 to 1|23.123.123.123")
            << "123.123.123.123" << 1 // start
            << false << Qt::Key_0 << '0'
            << "123.123.123.123" << 1 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("enter 0 to 12|3.123.123.123")
            << "123.123.123.123" << 2 // start
            << false << Qt::Key_0 << '0'
            << "123.123.123.123" << 2 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("enter 0 to 123.1|23.123.123")
            << "123.123.123.123" << 5 // start
            << false << Qt::Key_0 << '0'
            << "123.123.123.123" << 5 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("enter 0 to 123.12|3.123.123")
            << "123.123.123.123" << 6 // start
            << false << Qt::Key_0 << '0'
            << "123.123.123.123" << 6 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("enter 0 to 123.123.1|23.123")
            << "123.123.123.123" << 9 // start
            << false << Qt::Key_0 << '0'
            << "123.123.123.123" << 9 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("enter 0 to 123.123.12|3.123")
            << "123.123.123.123" << 10 // start
            << false << Qt::Key_0 << '0'
            << "123.123.123.123" << 10 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("enter 0 to 123.123.123.1|23")
            << "123.123.123.123" << 13 // start
            << false << Qt::Key_0 << '0'
            << "123.123.123.123" << 13 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("enter 0 to 123.123.123.12|3")
            << "123.123.123.123" << 14 // start
            << false << Qt::Key_0 << '0'
            << "123.123.123.123" << 14 // after click
            << "123.123.123.123"; // after finish

        // === enter non zero digit ===
        // enter 1 around zero octet
        QTest::newRow("enter 1 to |0.0.0.0")
            << "0.0.0.0" << 0 // start
            << false << Qt::Key_1 << '1'
            << "10.0.0.0" << 1 // after click
            << "10.0.0.0"; // after finish

        QTest::newRow("enter 1 to 0|.0.0.0")
            << "0.0.0.0" << 1 // start
            << false << Qt::Key_1 << '1'
            << "1.0.0.0" << 1 // after click
            << "1.0.0.0"; // after finish

        QTest::newRow("enter 1 to 0.|0.0.0")
            << "0.0.0.0" << 2 // start
            << false << Qt::Key_1 << '1'
            << "0.10.0.0" << 3 // after click
            << "0.10.0.0"; // after finish

        QTest::newRow("enter 1 to 0.0|.0.0")
            << "0.0.0.0" << 3 // start
            << false << Qt::Key_1 << '1'
            << "0.1.0.0" << 3 // after click
            << "0.1.0.0"; // after finish

        QTest::newRow("enter 1 to 0.0.|0.0")
            << "0.0.0.0" << 4 // start
            << false << Qt::Key_1 << '1'
            << "0.0.10.0" << 5 // after click
            << "0.0.10.0"; // after finish

        QTest::newRow("enter 1 to 0.0.0|.0")
            << "0.0.0.0" << 5 // start
            << false << Qt::Key_1 << '1'
            << "0.0.1.0" << 5 // after click
            << "0.0.1.0"; // after finish

        QTest::newRow("enter 1 to 0.0.0.|0")
            << "0.0.0.0" << 6 // start
            << false << Qt::Key_1 << '1'
            << "0.0.0.10" << 7 // after click
            << "0.0.0.10"; // after finish

        QTest::newRow("enter 1 to 0.0.0.0|")
            << "0.0.0.0" << 7 // start
            << false << Qt::Key_1 << '1'
            << "0.0.0.1" << 7 // after click
            << "0.0.0.1"; // after finish

        // enter 1 around two digit octet
        QTest::newRow("enter 1 to |12.12.12.12")
            << "12.12.12.12" << 0 // start
            << false << Qt::Key_1 << '1'
            << "112.12.12.12" << 1 // after click
            << "112.12.12.12"; // after finish

        QTest::newRow("enter 1 to 12|.12.12.12")
            << "12.12.12.12" << 2 // start
            << false << Qt::Key_1 << '1'
            << "121.12.12.12" << 3 // after click
            << "121.12.12.12"; // after finish

        QTest::newRow("enter 1 to 12.|12.12.12")
            << "12.12.12.12" << 3 // start
            << false << Qt::Key_1 << '1'
            << "12.112.12.12" << 4 // after click
            << "12.112.12.12"; // after finish

        QTest::newRow("enter 1 to 12.12|.12.12")
            << "12.12.12.12" << 5 // start
            << false << Qt::Key_1 << '1'
            << "12.121.12.12" << 6 // after click
            << "12.121.12.12"; // after finish

        QTest::newRow("enter 1 to 12.12.|12.12")
            << "12.12.12.12" << 6 // start
            << false << Qt::Key_1 << '1'
            << "12.12.112.12" << 7 // after click
            << "12.12.112.12"; // after finish

        QTest::newRow("enter 1 to 12.12.12|.12")
            << "12.12.12.12" << 8 // start
            << false << Qt::Key_1 << '1'
            << "12.12.121.12" << 9 // after click
            << "12.12.121.12"; // after finish

        QTest::newRow("enter 1 to 12.12.12.|12")
            << "12.12.12.12" << 9 // start
            << false << Qt::Key_1 << '1'
            << "12.12.12.112" << 10 // after click
            << "12.12.12.112"; // after finish

        QTest::newRow("enter 1 to 12.12.12.12|")
            << "12.12.12.12" << 11 // start
            << false << Qt::Key_1 << '1'
            << "12.12.12.121" << 12 // after click
            << "12.12.12.121"; // after finish

        // enter 1 to mid of two digit octet
        QTest::newRow("enter 1 to 1|2.12.12.12")
            << "12.12.12.12" << 1 // start
            << false << Qt::Key_1 << '1'
            << "112.12.12.12" << 2 // after click
            << "112.12.12.12"; // after finish

        QTest::newRow("enter 1 to 12.1|2.12.12")
            << "12.12.12.12" << 4 // start
            << false << Qt::Key_1 << '1'
            << "12.112.12.12" << 5 // after click
            << "12.112.12.12"; // after finish

        QTest::newRow("enter 1 to 12.12.1|2.12")
            << "12.12.12.12" << 7 // start
            << false << Qt::Key_1 << '1'
            << "12.12.112.12" << 8 // after click
            << "12.12.112.12"; // after finish

        QTest::newRow("enter 1 to 12.12.12.1|2")
            << "12.12.12.12" << 10 // start
            << false << Qt::Key_1 << '1'
            << "12.12.12.112" << 11 // after click
            << "12.12.12.112"; // after finish

        QTest::newRow("enter 1 to |10.10.10.10")
            << "10.10.10.10" << 0 // start
            << false << Qt::Key_1 << '1'
            << "110.10.10.10" << 1 // after click
            << "110.10.10.10"; // after finish

        QTest::newRow("enter 1 to 10|.10.10.10")
            << "10.10.10.10" << 2 // start
            << false << Qt::Key_1 << '1'
            << "101.10.10.10" << 3 // after click
            << "101.10.10.10"; // after finish

        QTest::newRow("enter 1 to 10.|10.10.10")
            << "10.10.10.10" << 3 // start
            << false << Qt::Key_1 << '1'
            << "10.110.10.10" << 4 // after click
            << "10.110.10.10"; // after finish

        QTest::newRow("enter 1 to 10.10|.10.10")
            << "10.10.10.10" << 5 // start
            << false << Qt::Key_1 << '1'
            << "10.101.10.10" << 6 // after click
            << "10.101.10.10"; // after finish

        QTest::newRow("enter 1 to 10.10.|10.10")
            << "10.10.10.10" << 6 // start
            << false << Qt::Key_1 << '1'
            << "10.10.110.10" << 7 // after click
            << "10.10.110.10"; // after finish

        QTest::newRow("enter 1 to 10.10.10|.10")
            << "10.10.10.10" << 8 // start
            << false << Qt::Key_1 << '1'
            << "10.10.101.10" << 9 // after click
            << "10.10.101.10"; // after finish

        QTest::newRow("enter 1 to 10.10.10.|10")
            << "10.10.10.10" << 9 // start
            << false << Qt::Key_1 << '1'
            << "10.10.10.110" << 10 // after click
            << "10.10.10.110"; // after finish

        QTest::newRow("enter 1 to 10.10.10.10|")
            << "10.10.10.10" << 11 // start
            << false << Qt::Key_1 << '1'
            << "10.10.10.101" << 12 // after click
            << "10.10.10.101"; // after finish

        // enter 1 to mid of two digit octet
        QTest::newRow("enter 1 to 1|2.12.12.12")
            << "12.12.12.12" << 1 // start
            << false << Qt::Key_1 << '1'
            << "112.12.12.12" << 2 // after click
            << "112.12.12.12"; // after finish

        QTest::newRow("enter 1 to 12.1|2.12.12")
            << "12.12.12.12" << 4 // start
            << false << Qt::Key_1 << '1'
            << "12.112.12.12" << 5 // after click
            << "12.112.12.12"; // after finish

        QTest::newRow("enter 1 to 12.12.1|2.12")
            << "12.12.12.12" << 7 // start
            << false << Qt::Key_1 << '1'
            << "12.12.112.12" << 8 // after click
            << "12.12.112.12"; // after finish

        QTest::newRow("enter 1 to 12.12.12.1|2")
            << "12.12.12.12" << 10 // start
            << false << Qt::Key_1 << '1'
            << "12.12.12.112" << 11 // after click
            << "12.12.12.112"; // after finish

        QTest::newRow("enter 1 to 1|0.10.10.10")
            << "10.10.10.10" << 1 // start
            << false << Qt::Key_1 << '1'
            << "110.10.10.10" << 2 // after click
            << "110.10.10.10"; // after finish

        QTest::newRow("enter 1 to 10.1|0.10.10")
            << "10.10.10.10" << 4 // start
            << false << Qt::Key_1 << '1'
            << "10.110.10.10" << 5 // after click
            << "10.110.10.10"; // after finish

        QTest::newRow("enter 1 to 10.10.1|0.10")
            << "10.10.10.10" << 7 // start
            << false << Qt::Key_1 << '1'
            << "10.10.110.10" << 8 // after click
            << "10.10.110.10"; // after finish

        QTest::newRow("enter 1 to 10.10.10.1|0")
            << "10.10.10.10" << 10 // start
            << false << Qt::Key_1 << '1'
            << "10.10.10.110" << 11 // after click
            << "10.10.10.110"; // after finish

        // enter 1 around three digit octet
        QTest::newRow("enter 1 to |100.100.100.100")
            << "100.100.100.100" << 0 // start
            << false << Qt::Key_1 << '1'
            << "100.100.100.100" << 0 // after click
            << "100.100.100.100"; // after finish

        QTest::newRow("enter 1 to 100|.100.100.100")
            << "100.100.100.100" << 3 // start
            << false << Qt::Key_1 << '1'
            << "100.100.100.100" << 3 // after click
            << "100.100.100.100"; // after finish

        QTest::newRow("enter 1 to 100.|100.100.100")
            << "100.100.100.100" << 4 // start
            << false << Qt::Key_1 << '1'
            << "100.100.100.100" << 4 // after click
            << "100.100.100.100"; // after finish

        QTest::newRow("enter 1 to 100.100|.100.100")
            << "100.100.100.100" << 7 // start
            << false << Qt::Key_1 << '1'
            << "100.100.100.100" << 7 // after click
            << "100.100.100.100"; // after finish

        QTest::newRow("enter 1 to 100.100.|100.100")
            << "100.100.100.100" << 8 // start
            << false << Qt::Key_1 << '1'
            << "100.100.100.100" << 8 // after click
            << "100.100.100.100"; // after finish

        QTest::newRow("enter 1 to 100.100.100|.100")
            << "100.100.100.100" << 11 // start
            << false << Qt::Key_1 << '1'
            << "100.100.100.100" << 11 // after click
            << "100.100.100.100"; // after finish

        QTest::newRow("enter 1 to 100.100.100.|100")
            << "100.100.100.100" << 12 // start
            << false << Qt::Key_1 << '1'
            << "100.100.100.100" << 12 // after click
            << "100.100.100.100"; // after finish

        QTest::newRow("enter 1 to 100.100.100.100|")
            << "100.100.100.100" << 15 // start
            << false << Qt::Key_1 << '1'
            << "100.100.100.100" << 15 // after click
            << "100.100.100.100"; // after finish

        // enter 1 to mid of three digit octet
        QTest::newRow("enter 1 to 1|00.100.100.100")
            << "100.100.100.100" << 1 // start
            << false << Qt::Key_1 << '1'
            << "100.100.100.100" << 1 // after click
            << "100.100.100.100"; // after finish

        QTest::newRow("enter 1 to 10|0.100.100.100")
            << "100.100.100.100" << 2 // start
            << false << Qt::Key_1 << '1'
            << "100.100.100.100" << 2 // after click
            << "100.100.100.100"; // after finish

        QTest::newRow("enter 1 to 100.1|00.100.100")
            << "100.100.100.100" << 5 // start
            << false << Qt::Key_1 << '1'
            << "100.100.100.100" << 5 // after click
            << "100.100.100.100"; // after finish

        QTest::newRow("enter 1 to 100.10|0.100.100")
            << "100.100.100.100" << 6 // start
            << false << Qt::Key_1 << '1'
            << "100.100.100.100" << 6 // after click
            << "100.100.100.100"; // after finish

        QTest::newRow("enter 1 to 100.100.1|00.100")
            << "100.100.100.100" << 9 // start
            << false << Qt::Key_1 << '1'
            << "100.100.100.100" << 9 // after click
            << "100.100.100.100"; // after finish

        QTest::newRow("enter 1 to 100.100.10|0.100")
            << "100.100.100.100" << 10 // start
            << false << Qt::Key_1 << '1'
            << "100.100.100.100" << 10 // after click
            << "100.100.100.100"; // after finish

        QTest::newRow("enter 1 to 100.100.100.1|00")
            << "100.100.100.100" << 13 // start
            << false << Qt::Key_1 << '1'
            << "100.100.100.100" << 13 // after click
            << "100.100.100.100"; // after finish

        QTest::newRow("enter 1 to 100.100.100.10|0")
            << "100.100.100.100" << 14 // start
            << false << Qt::Key_1 << '1'
            << "100.100.100.100" << 14 // after click
            << "100.100.100.100"; // after finish

        // === max octet value ===
        // enter max valid octet value
        QTest::newRow("enter 5 to 2|5.25.25.25")
            << "25.25.25.25" << 1 // start
            << false << Qt::Key_5 << '5'
            << "255.25.25.25" << 2 // after click
            << "255.25.25.25"; // after finish

        QTest::newRow("enter 5 to 25|.25.25.25")
            << "25.25.25.25" << 2 // start
            << false << Qt::Key_5 << '5'
            << "255.25.25.25" << 3 // after click
            << "255.25.25.25"; // after finish

        QTest::newRow("enter 5 to 25.2|5.25.25")
            << "25.25.25.25" << 4 // start
            << false << Qt::Key_5 << '5'
            << "25.255.25.25" << 5 // after click
            << "25.255.25.25"; // after finish

        QTest::newRow("enter 5 to 25.25|.25.25")
            << "25.25.25.25" << 5 // start
            << false << Qt::Key_5 << '5'
            << "25.255.25.25" << 6 // after click
            << "25.255.25.25"; // after finish

        QTest::newRow("enter 5 to 25.25.2|5.25")
            << "25.25.25.25" << 7 // start
            << false << Qt::Key_5 << '5'
            << "25.25.255.25" << 8 // after click
            << "25.25.255.25"; // after finish

        QTest::newRow("enter 5 to 25.25.25|.25")
            << "25.25.25.25" << 8 // start
            << false << Qt::Key_5 << '5'
            << "25.25.255.25" << 9 // after click
            << "25.25.255.25"; // after finish

        QTest::newRow("enter 5 to 25.25.25.2|5")
            << "25.25.25.25" << 10 // start
            << false << Qt::Key_5 << '5'
            << "25.25.25.255" << 11 // after click
            << "25.25.25.255"; // after finish

        QTest::newRow("enter 5 to 25.25.25.25|")
            << "25.25.25.25" << 11 // start
            << false << Qt::Key_5 << '5'
            << "25.25.25.255" << 12 // after click
            << "25.25.25.255"; // after finish

        QTest::newRow("enter 2 to |55.55.55.55")
            << "55.55.55.55" << 0 // start
            << false << Qt::Key_2 << '2'
            << "255.55.55.55" << 1 // after click
            << "255.55.55.55"; // after finish

        QTest::newRow("enter 2 to 55.|55.55.55")
            << "55.55.55.55" << 3 // start
            << false << Qt::Key_2 << '2'
            << "55.255.55.55" << 4 // after click
            << "55.255.55.55"; // after finish

        QTest::newRow("enter 2 to 55.55.|55.55")
            << "55.55.55.55" << 6 // start
            << false << Qt::Key_2 << '2'
            << "55.55.255.55" << 7 // after click
            << "55.55.255.55"; // after finish

        QTest::newRow("enter 2 to 55.55.55.|55")
            << "55.55.55.55" << 9 // start
            << false << Qt::Key_2 << '2'
            << "55.55.55.255" << 10 // after click
            << "55.55.55.255"; // after finish

        // enter overflowed octet value

        // === special chars
        // before dot
        // after dot
        // between digits

        // === enter to int ===
        // enter zero to int value
        // enter non zero digit to int value
        // enter special char to int value

        // === max int value ===
        // enter max valid int value
        // enter overflowed int value

        // === remove by delete ===
        // rermove dot
        QTest::newRow("delete in 0|.0.0.0")
            << "0.0.0.0" << 1 // start
            << false << delete_key << '>'
            << "0.0.0.0" << 2 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("delete in 0.0|.0.0")
            << "0.0.0.0" << 3 // start
            << false << delete_key << '>'
            << "0.0.0.0" << 4 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("delete in 0.0.0|.0")
            << "0.0.0.0" << 5 // start
            << false << delete_key << '>'
            << "0.0.0.0" << 6 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("delete in 1|.1.1.1")
            << "1.1.1.1" << 1 // start
            << false << delete_key << '>'
            << "1.1.1.1" << 2 // after click
            << "1.1.1.1"; // after finish

        QTest::newRow("delete in 1.1|.1.1")
            << "1.1.1.1" << 3 // start
            << false << delete_key << '>'
            << "1.1.1.1" << 4 // after click
            << "1.1.1.1"; // after finish

        QTest::newRow("delete in 1.1.1|.1")
            << "1.1.1.1" << 5 // start
            << false << delete_key << '>'
            << "1.1.1.1" << 6 // after click
            << "1.1.1.1"; // after finish

        QTest::newRow("delete in 12|.12.12.12")
            << "12.12.12.12" << 2 // start
            << false << delete_key << '>'
            << "12.12.12.12" << 3 // after click
            << "12.12.12.12"; // after finish

        QTest::newRow("delete in 12.12|.12.12")
            << "12.12.12.12" << 5 // start
            << false << delete_key << '>'
            << "12.12.12.12" << 6 // after click
            << "12.12.12.12"; // after finish

        QTest::newRow("delete in 12.12.12|.12")
            << "12.12.12.12" << 8 // start
            << false << delete_key << '>'
            << "12.12.12.12" << 9 // after click
            << "12.12.12.12"; // after finish

        QTest::newRow("delete in 123|.123.123.123")
            << "123.123.123.123" << 3 // start
            << false << delete_key << '>'
            << "123.123.123.123" << 4 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("delete in 123.123|.123.123")
            << "123.123.123.123" << 7 // start
            << false << delete_key << '>'
            << "123.123.123.123" << 8 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("delete in 123.123.123|.123")
            << "123.123.123.123" << 11 // start
            << false << delete_key << '>'
            << "123.123.123.123" << 12 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("delete in |...0")
            << "...0" << 0 // start
            << false << delete_key << '>'
            << "...0" << 1 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("delete in .|..0")
            << "...0" << 1 // start
            << false << delete_key << '>'
            << "...0" << 2 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("delete in ..|.0")
            << "...0" << 2 // start
            << false << delete_key << '>'
            << "...0" << 3 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("delete in |..0.")
            << "..0." << 0 // start
            << false << delete_key << '>'
            << "..0." << 1 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("delete in .|.0.")
            << "..0." << 1 // start
            << false << delete_key << '>'
            << "..0." << 2 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("delete in ..0|.")
            << "..0." << 3 // start
            << false << delete_key << '>'
            << "..0." << 4 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("delete in |.0..")
            << ".0.." << 0 // start
            << false << delete_key << '>'
            << ".0.." << 1 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("delete in .0|..")
            << ".0.." << 2 // start
            << false << delete_key << '>'
            << ".0.." << 3 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("delete in .0.|.")
            << ".0.." << 3 // start
            << false << delete_key << '>'
            << ".0.." << 4 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("delete in 0|...")
            << "0..." << 1 // start
            << false << delete_key << '>'
            << "0..." << 2 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("delete in 0.|..")
            << "0..." << 2 // start
            << false << delete_key << '>'
            << "0..." << 3 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("delete in 0..|.")
            << "0..." << 3 // start
            << false << delete_key << '>'
            << "0..." << 4 // after click
            << "0.0.0.0"; // after finish

        // rermove zero octet
        // remove one digit octet
        // remove start of two digit octet
        // remove end of two digit octet
        // remove start of three digit octet
        // remove end of three digit octet
        // remove mid of three digit octet
        // remove start of int value
        // remove mid of int value
        // remove end of int value

        // === remove by backspace ===
        // rermove dot
        QTest::newRow("backspace in 0.|0.0.0")
            << "0.0.0.0" << 2 // start
            << false << backspace_key << '<'
            << "0.0.0.0" << 1 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("backspace in 0.0.|0.0")
            << "0.0.0.0" << 4 // start
            << false << backspace_key << '<'
            << "0.0.0.0" << 3 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("backspace in 0.0.0.|0")
            << "0.0.0.0" << 6 // start
            << false << backspace_key << '<'
            << "0.0.0.0" << 5 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("backspace in 1.|1.1.1")
            << "1.1.1.1" << 2 // start
            << false << backspace_key << '<'
            << "1.1.1.1" << 1 // after click
            << "1.1.1.1"; // after finish

        QTest::newRow("backspace in 1.1.|1.1")
            << "1.1.1.1" << 4 // start
            << false << backspace_key << '<'
            << "1.1.1.1" << 3 // after click
            << "1.1.1.1"; // after finish

        QTest::newRow("backspace in 1.1.1.|1")
            << "1.1.1.1" << 6 // start
            << false << backspace_key << '<'
            << "1.1.1.1" << 5 // after click
            << "1.1.1.1"; // after finish

        QTest::newRow("backspace in 12.|12.12.12")
            << "12.12.12.12" << 3 // start
            << false << backspace_key << '<'
            << "12.12.12.12" << 2 // after click
            << "12.12.12.12"; // after finish

        QTest::newRow("backspace in 12.12.|12.12")
            << "12.12.12.12" << 6 // start
            << false << backspace_key << '<'
            << "12.12.12.12" << 5 // after click
            << "12.12.12.12"; // after finish

        QTest::newRow("backspace in 12.12.12.|12")
            << "12.12.12.12" << 9 // start
            << false << backspace_key << '<'
            << "12.12.12.12" << 8 // after click
            << "12.12.12.12"; // after finish

        QTest::newRow("backspace in 123.|123.123.123")
            << "123.123.123.123" << 4 // start
            << false << backspace_key << '<'
            << "123.123.123.123" << 3 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("backspace in 123.123.|123.123")
            << "123.123.123.123" << 8 // start
            << false << backspace_key << '<'
            << "123.123.123.123" << 7 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("backspace in 123.123.123.|123")
            << "123.123.123.123" << 12 // start
            << false << backspace_key << '<'
            << "123.123.123.123" << 11 // after click
            << "123.123.123.123"; // after finish

        QTest::newRow("backspace in .|..0")
            << "...0" << 1 // start
            << false << backspace_key << '<'
            << "...0" << 0 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("backspace in ..|.0")
            << "...0" << 2 // start
            << false << backspace_key << '<'
            << "...0" << 1 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("backspace in ...|0")
            << "...0" << 3 // start
            << false << backspace_key << '<'
            << "...0" << 2 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("backspace in .|.0.")
            << "..0." << 1 // start
            << false << backspace_key << '<'
            << "..0." << 0 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("backspace in ..|0.")
            << "..0." << 2 // start
            << false << backspace_key << '<'
            << "..0." << 1 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("backspace in ..0.|")
            << "..0." << 4 // start
            << false << backspace_key << '<'
            << "..0." << 3 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("backspace in .|0..")
            << ".0.." << 1 // start
            << false << backspace_key << '<'
            << ".0.." << 0 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("backspace in .0.|.")
            << ".0.." << 3 // start
            << false << backspace_key << '<'
            << ".0.." << 2 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("backspace in .0..|")
            << ".0.." << 4 // start
            << false << backspace_key << '<'
            << ".0.." << 3 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("backspace in 0.|..")
            << "0..." << 2 // start
            << false << backspace_key << '<'
            << "0..." << 1 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("backspace in 0..|.")
            << "0..." << 3 // start
            << false << backspace_key << '<'
            << "0..." << 2 // after click
            << "0.0.0.0"; // after finish

        QTest::newRow("backspace in 0...|")
            << "0..." << 4 // start
            << false << backspace_key << '<'
            << "0..." << 3 // after click
            << "0.0.0.0"; // after finish

        // rermove zero octet
        // remove one digit octet
        // remove start of two digit octet
        // remove end of two digit octet
        // remove start of three digit octet
        // remove end of three digit octet
        // remove mid of three digit octet
        // remove start of int value
        // remove mid of int value
        // remove end of int value
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
