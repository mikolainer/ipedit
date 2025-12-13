#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QValidator>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QString>

#include "testrunner.h"

class IpV4
{
public:
    class Octet
    {
        const QString m_text;

    public:
        Octet(const QString& value) : m_text(value) {};
        static bool is_valid(int value)
        {
            return value >= min_value && value <= max_value;
        }

        bool is_valid() const
        {
            if (m_text.isEmpty())
                return false;

            if (m_text.contains(QRegularExpression("\\D")))
                return false;

            int value = int_value();
            if (!is_valid(value))
                return false;

            if (value > 0 && m_text.startsWith("0"))
                return false;

            if (value == 0 && m_text != "0")
                return false;

            return true;
        }

    private:
        int int_value() const
        {
            bool ok;
            const int value = m_text.toInt(&ok);
            return ok ? value : min_value-1;
        }

    public:
        constexpr static const int min_value{0};
        constexpr static const int max_value{255};
    };

public:
    IpV4(const QString& value) : m_text(value) {};
    bool is_valid() const
    {
        int separators_count = m_text.count(octet_separator);
        if (separators_count != norm_separators_count)
            return false;

        for (const QString& octet : m_text.split(octet_separator))
            if (!Octet(octet).is_valid())
                return false;

        return true;
    }

    static bool is_valid(qint64 value)
    {
        return value >= min_intvalue && value <= max_intvalue;
    }

private:
    const QString m_text;

public:
    constexpr static const int norm_separators_count {3};
    constexpr static const int norm_octets_count {norm_separators_count + 1};
    constexpr static const char octet_separator{'.'};

    constexpr static const qint64 min_intvalue{0x00000000};
    constexpr static const qint64 max_intvalue{0xFFFFFFFF};
};

class TotalFixup
{
public:
    static void convert_from_int(QString& text);
    static void normalize_separators(QString& text);

private:
    static void move_dots(QString &text);
    static void strip_end(QString &text);
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
    bool is_valid(const QString &text);
    bool is_invalid(const QString &text);
    void fix_removed_separator(QString &text, int &pos, bool is_removing_direction_backward) const;
    void update(const QString &text, const int &pos);
    bool is_removed_manually(const QString &text, const int &pos, bool* is_backward_direction = nullptr, QChar* removed_char = nullptr) const;
    bool is_inserted_manually(const QString &text, QChar* inserted_char = nullptr) const;

private:
    int m_last_pos = 0;
    QString m_last_text = default_valid_value;

public:
    constexpr static const int max_octet_value {255};
    constexpr static const int norm_separators_count {3};
    constexpr static const int norm_octets_count {norm_separators_count + 1};
    constexpr static const char octet_separator{'.'};
    constexpr static const char invalid_octet_start{'0'};
    constexpr static const char default_valid_octet{'0'};
    constexpr static const char* default_valid_value{"0.0.0.0"};

    constexpr static const char* available_chars {"1234567890"};
    constexpr static const char* special_chars {" .,/?ю"};
};


class OctetFixup
{
public:
    static bool fix_empty(QString& octet);
    static int fix_start(QString& octet);
};

class MainWindow : public QWidget
{ Q_OBJECT
public:
    MainWindow() : QWidget() {
        {DirtyTestRunner();}

        auto main_lay = new QVBoxLayout(this);
        auto ip_edit = new QLineEdit("0.0.0.0", this);
        IntIpValidator::set_to(ip_edit);
        main_lay->addWidget(ip_edit);

    }
};
#endif // MAINWINDOW_H
