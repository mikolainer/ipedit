#ifndef INTIPVALIDATOR_H
#define INTIPVALIDATOR_H

#include <QValidator>

#include "textchangehistory.h"
#include "texteditstate.h"

class QLineEdit;

class IpChangeAnalyzer
{
public:
    static TextEditState prepare_new_state(const TextEditState& prev, const TextEditState& cur);
};

class IntIpValidator : public QValidator
{ Q_OBJECT

private:
    IntIpValidator(QObject* parent);

public:
    static void set_to(QLineEdit* editor = nullptr);

    virtual State validate(QString &text, int &pos) const override;
    virtual void fixup(QString &text) const override;

private:
    void update(const int &pos);
    void update(const QLineEdit& edit);

private:
    TextChangeHistory m_text;
};

#endif // INTIPVALIDATOR_H
