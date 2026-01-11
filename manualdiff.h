#ifndef MANUALDIFF_H
#define MANUALDIFF_H

#include <QChar>
#include "texteditstate.h"

class ManualDiff{
public:
    enum RemoveDirection{
        Forward, // by Delete
        Backward // by Backspace
    };
    
    bool inserted = false;
    bool removed = false;
    RemoveDirection remove_dir = Forward;
    QChar ch = QChar::Null;
    int index = -1;
    
private:
    const TextEditState m_prev;
    const TextEditState m_cur;
    
public:
    ManualDiff(const TextEditState& prev, const TextEditState& cur);
    
    TextEditState fixup_removed_separator() const;
    TextEditState fixup_inserted_separator() const;
    TextEditState fixup_inserted_zero() const;
    TextEditState fixup_empty_field() const;
    
    bool is_insert_insignificant_zero() const;
    bool valid() const;

private:
    void init_removed();
    void init_inserted();
};

#endif // MANUALDIFF_H
