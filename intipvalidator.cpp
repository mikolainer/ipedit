#include "intipvalidator.h"
#include "intipv4calculator.h"

#include <QLineEdit>

#include "ipv4int.h"
#include "ipv4.h"

IntIpValidator::IntIpValidator(QObject *parent) : QValidator(parent) {}

void IntIpValidator::set_to(QLineEdit *editor){
    if (editor){
        editor->clear();

        auto validator = new IntIpValidator(editor);
        editor->setValidator(validator);

        connect(
            editor, &QLineEdit::cursorPositionChanged,
            validator, [validator](int old_pos, int new_pos){
                Q_UNUSED(old_pos)
                validator->update(new_pos);
            }
        );

        validator->update(*editor);
    }
}

QValidator::State IntIpValidator::validate(QString &text, int &pos) const
{
    try{
        const TextEditState previous_state = m_history.cur_state();
        const TextEditState current_state{text, pos};
        const TextEditState new_state = IpChangeAnalyzer::prepare_new_state(previous_state, current_state);

        text = new_state.val;
        pos = new_state.pos;
        const_cast<TextChangeHistory&>(m_history).update(text, pos);

        IpV4 ip{text};
        return ip.is_valid() ? Acceptable : Intermediate;
    }
    catch(const std::exception& e){
        return Invalid;
    }
}

void IntIpValidator::fixup(QString &text) const {
    if (text.isEmpty())
    {
        text = "0.0.0.0";
        return;
    }

    const IpV4Int int_ip = {text};
    if (int_ip.is_valid())
        text = IntIpV4Calculator::to_ipv4(text);
    else
        text = IpV4::fix(text);
}

void IntIpValidator::update(const int &pos)
{
    m_history.update(pos);
}

void IntIpValidator::update(const QLineEdit &edit)
{
    m_history.update(edit);
}

TextEditState IpChangeAnalyzer::prepare_new_state(const TextEditState &prev, const TextEditState &cur)
{
    TextEditState result = cur;
    result.fixup_wrong_separators();
    result.fixup_manual_changes(prev);
    result.fixup_insignificant_zeros();

    if (result.is_valid())
        return result;

    if (result.is_prevalid())
        return result;

    throw std::runtime_error("can't fixup the input");
}
