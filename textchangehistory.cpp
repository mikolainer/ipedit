
#include "textchangehistory.h"

TextChangeHistory::TextChangeHistory(const QString &text, const int &pos)
    :m_val({text}), m_pos({pos})
{}

bool TextChangeHistory::update(const int &pos){
    update_text(cur_value());
    update_pos(pos);

    checkup();
    return true;
}

bool TextChangeHistory::update(const QString &text, const int &pos){
    if (cur_value() == text && cur_pos() == pos)
        return false;

    update_text(text);
    update_pos(pos);

    checkup();
    return true;
}

bool TextChangeHistory::update(const QLineEdit &edit){
    return update(edit.text(), edit.cursorPosition());
}

bool TextChangeHistory::revert(){
    bool ok = true;

    if (m_pos.length() > 1)
        m_pos.removeFirst();
    else
        ok = false;

    if (m_val.length() > 1)
        m_val.removeFirst();
    else
        ok = false;

    return ok;
}

int TextChangeHistory::prev_pos() const {
    if (m_pos.length() <= 1)
        throw std::runtime_error("prev state not found!");

    return m_pos[1];
}

QString TextChangeHistory::prev_value() const {
    if (m_pos.length() <= 1)
        throw std::runtime_error("prev state not found!");

    return m_val[1];
}

int TextChangeHistory::cur_pos() const {
    return m_pos[0];
}

QString TextChangeHistory::cur_value() const {
    return m_val[0];
}

TextEditState TextChangeHistory::cur_state() const {
    return {cur_value(), cur_pos()};
}

TextEditState TextChangeHistory::prev_state() const {
    return {prev_value(), prev_pos()};
}

void TextChangeHistory::update_text(const QString &text)
{
    m_val.prepend(text);

    if (m_val.length() > history_size)
        m_val.removeLast();
}

void TextChangeHistory::update_pos(const int &pos)
{
    m_pos.prepend(pos);

    if (m_pos.length() > history_size)
        m_pos.removeLast();
}

void TextChangeHistory::checkup(){
    Q_ASSERT(m_pos.length() == m_val.length());
}
