#include "mainwindow.h"
#include "oneclick_testfactory.h"

void IpEditTestHelper::start_edit(int cursor_pos){
    setFocus();

    const CursorPos cursor(text());
    if (cursor.is_valid(cursor_pos))
        setCursorPosition(cursor_pos);
}

void IpEditTestHelper::finish_edit()
{
    QTest::keyClick(this, Qt::Key_Enter);
}

void IpEditTestHelper::check(const QString &expected_text, int expected_pos) {
    QCOMPARE(text(), expected_text);
    check(expected_pos);
}

void IpEditTestHelper::check(const QString &expected_finish_text){
    QCOMPARE(text(), expected_finish_text);
}

void IpEditTestHelper::check(int expected_pos){
    QCOMPARE(cursorPosition(), expected_pos);
}

void IpEditTestHelper::click(char key){
    QTest::keyClick(this, key, kb_modifier, typing_delay);
}

void IpEditTestHelper::click(Qt::Key key){
    QTest::keyClick(this, key, kb_modifier, typing_delay);
}

IpEditTestHelper::IpEditTestHelper(const QString &text, QWidget *parent)
    : QLineEdit(parent)
{
    IntIpValidator::set_to(this);
    setText(text);
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

OneClickTestFactory::OneClickTestFactory() : TestDataFactory() {}

void OneClickTestFactory::make_schema() const
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

void OneClickTestFactory::fill_data(const Case &test) const
{
    QTest::newRow(test.name.toUtf8().data())
    << test.start.value << test.start.pos // start
    << test.key.is_char << test.key.click_key << test.key.click_char
    << test.expected.value << test.expected.pos // after click
    << test.finish_value; // after finish
}

void TestDataFactory::fill_data(const QList<Case> &tests, const QString *finish_value){
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
}

ReplaceBefore::ReplaceBefore(const Click &key) : ClickEffect(key) {
    Q_ASSERT(!key.is_backward_remove() && !key.is_forward_remove());
}

State ReplaceBefore::new_value(const State &start) const{
    Q_ASSERT(start.pos > 0);

    auto start_copy = start;
    *(start_copy.value.begin() + start.pos -1) = m_key.click_char;
    return start_copy;
}

QString ReplaceBefore::finish_value(const QString &expected) const{
    return expected;
}

InsertOver::InsertOver(const Click &key) : ClickEffect(key) {
    Q_ASSERT(!key.is_backward_remove() && !key.is_forward_remove());
}

State InsertOver::new_value(const State &start) const{
    auto start_copy = start;
    start_copy.pos += 1;
    *(start_copy.value.begin() + start.pos) = m_key.click_char;
    return start_copy;
}

QString InsertOver::finish_value(const QString &expected) const{
    return expected;
}

State OnlyPosMove::new_value(const State &start) const{
    auto start_copy = start;
    start_copy.pos += m_key.is_backward_remove() ? -1 : +1;
    return start_copy;
}

QString OnlyPosMove::finish_value(const QString &expected) const{
    return expected;
}

State Invalid::new_value(const State &start) const{
    return start;
}

QString Invalid::finish_value(const QString &expected) const{
    return expected;
}

State Valid::new_value(const State &start) const{
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

QString Valid::finish_value(const QString &expected) const{
    return expected;
}

QList<Case> ClickEffect::make(const InputSet &format) const
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
}

Click::Click(char key)
    : is_char(true)
    , click_key(Click::default_key)
    , click_char(key)
{}

Click::Click(Qt::Key key)
    : is_char(false)
    , click_key(key)
    , click_char(char_from_key(key))
{}

Click::Click(bool is_char, Qt::Key key, char ch)
    : is_char(is_char)
    , click_key(key)
    , click_char(ch)
{}

Click::Click()
    : is_char(false)
    , click_key(Click::default_key)
    , click_char(Click::default_char)
{}

char Click::char_from_key(Qt::Key key)
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

bool Click::is_forward_remove() const {
    return click_key == Qt::Key_Delete;
}

bool Click::is_backward_remove() const {
    return click_key == Qt::Key_Backspace;
}

QString Click::make_test_name(const State &start_state) const
{
    bool is_remove = is_backward_remove() || is_forward_remove();

    return QString("%1 %2 %3")
        .arg(
            operation_name(),
            is_remove ? "in" : QString("`%1` to").arg(click_char),
            start_state.toString()
            );
}

QString Click::operation_name() const{
    if (is_backward_remove())
        return "backspace";

    if (is_forward_remove())
        return "delete";

    return "enter";
}
