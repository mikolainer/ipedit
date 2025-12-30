#ifndef INTIPVALIDATOR_H
#define INTIPVALIDATOR_H

#include <QValidator>

class QLineEdit;

class IntIpValidator : public QValidator
{ Q_OBJECT

private:
    IntIpValidator(QObject* parent);

public:
    static void set_to(QLineEdit* editor = nullptr);

    virtual State validate(QString &text, int &pos) const override;
    virtual void fixup(QString &text) const override;

private:
    void fix_removed_separator(QString &text, int &pos, bool is_removing_direction_backward) const;
    void update(const QString &text, const int &pos);
    bool is_removed_manually(const QString &text, const int &pos, bool* is_backward_direction = nullptr, QChar* removed_char = nullptr) const;
    bool is_inserted_manually(const QString &text, QChar* inserted_char = nullptr) const;

private:
    int m_last_pos;
    QString m_last_text;

public:
    constexpr static const char* available_chars {"1234567890"};
    constexpr static const char* special_chars {" .,/?ю"};
};

#endif // INTIPVALIDATOR_H
