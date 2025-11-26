#include "test.h"
#include "mainwindow.h"

void LineEditWithIpValidatorTest::start_edit(int cursor_pos){
    setFocus();

    const CursorPos cursor(text());
    if (cursor.is_valid(cursor_pos))
        setCursorPosition(cursor_pos);
}

void LineEditWithIpValidatorTest::finish_edit()
{
    QTest::keyClick(this, Qt::Key_Enter);
}

void Test_IpEdit::clear_all_by_backspace_0_0_0_0()
{
    auto ip_edit = std::make_unique<LineEditWithIpValidatorTest>("0.0.0.0");
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
    auto ip_edit = std::make_unique<LineEditWithIpValidatorTest>("0.0.0.0");
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

void DirtyTestRunner::run_all()
{
    {
        auto test = Test_OneClickHelpers();
        QTest::qExec(&test);
    }
    {
        auto test = Test_IpEdit();
        QTest::qExec(&test);
    }
}

LineEditWithIpValidatorTest::LineEditWithIpValidatorTest(const QString &text, QWidget *parent)
    : QLineEdit(text, parent)
{
    IntIpValidator::set_to(this);
}


ClickEffect::InputSet const ClickEffect::InputSet::make(const QString &format)
{
    InputSet result;
    auto it = format.begin();
    const auto the_end = format.end();

    int current_pos = 0;
    while (it != the_end){
        QChar ch = *it;
        if (ch == click_pos_marker){
            result.click_pos.append(current_pos);
        }else{
            ++current_pos;
            result.start_value.append(ch);
        }
        ++it;
    }

    Q_ASSERT(result.click_pos.count() > 0);
    return result;
}

QList<State> ClickEffect::InputSet::start_states() const {
    QList<State> result;
    for (const int start_pos : click_pos){
        State start_state;
        start_state.value = start_value;
        start_state.pos = start_pos;
        result.append(start_state);
    }

    return result;
}

QString State::toString() const {
    auto value_copy = value;
    return value_copy.insert(pos, ClickEffect::InputSet::click_pos_marker);
}
