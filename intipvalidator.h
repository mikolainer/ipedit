#ifndef INTIPVALIDATOR_H
#define INTIPVALIDATOR_H

#include <QValidator>

#include "global.h"
#include "texteditstate.h"
#include "textchangehistory.h"

class QLineEdit;

class IpChangeAnalyzer
{
public:
    static TextEditState prepare_new_state(const TextEditState& prev, const TextEditState& cur);
};

class LIB_EXPORT IntIpValidator : public QValidator
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
    TextChangeHistory m_history;
};

#endif // INTIPVALIDATOR_H
