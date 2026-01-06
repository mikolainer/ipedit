#ifndef TEXTCHANGEHISTORY_H
#define TEXTCHANGEHISTORY_H

#include <QString>
#include <QLineEdit>

#include "texteditstate.h"

class TextChangeHistory
{
public:
    TextChangeHistory(const QString &text = QString(), const int &pos = 0);
    
    bool update(const int &pos);
    bool update(const QString &text, const int &pos);
    bool update(const QLineEdit& edit);
    bool revert();
    int prev_pos() const;
    QString prev_value() const;
    int cur_pos() const;
    QString cur_value() const;
    TextEditState cur_state() const;
    TextEditState prev_state() const;
    
private:
    void update_text(const QString &text);
    void update_pos(const int &pos);
    void checkup();
    
private:
    QList<QString> m_val;
    QList<int> m_pos;
    
    constexpr static const int history_size {3};
};

#endif // TEXTCHANGEHISTORY_H
