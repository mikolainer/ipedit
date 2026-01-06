#ifndef TEXTEDITSTATE_H
#define TEXTEDITSTATE_H

#include <QString>

struct TextEditState{
    QString val;
    int pos;
    
public:
    void fixup_wrong_separators();
    void fixup_insignificant_zeros();
    void fixup_manual_changes(const TextEditState &prev);
    bool have_invalid_chars() const;
    bool is_valid() const;
    bool is_prevalid() const;
    bool is_invalid() const;
};

#endif // TEXTEDITSTATE_H
