#include "intipedit.h"
#include "intipvalidator.h"
#include "intipv4calculator.h"
#include "ipv4.h"
#include <QVBoxLayout>

IntIpEdit::IntIpEdit(QWidget *parent)
    : IntIpEdit(default_value, parent)
{}

IntIpEdit::IntIpEdit(const QString& text, QWidget *parent)
    : QWidget(parent)
{
    IntIpValidator::set_to(&m_edit);
    m_edit.setText(text);
    connect(&m_edit, &QLineEdit::textChanged, this, &IntIpEdit::ipChanged);

    setup_ui();
}

QString IntIpEdit::getIp() const
{
    return IpV4::fix(m_edit.text());
}

void IntIpEdit::setIp(const QString &text)
{
    m_edit.setText(text);
}

void IntIpEdit::ipChanged(const QString &text)
{
    m_edit.setToolTip(IntIpV4Calculator::convert(text));
    emit onIpChanged(getIp());
}

void IntIpEdit::setup_ui()
{
    auto main_lay = new QVBoxLayout(this);
    main_lay->addWidget(&m_edit);
    main_lay->setContentsMargins(0,0,0,0);
}
