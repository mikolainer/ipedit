#ifndef IPV4_H
#define IPV4_H

#include <QString>

class IpV4
{
public:
    class Octet
    {
    public:
        static bool fix_empty(QString& octet);
        static int fix_start(QString& octet);
        
    public:
        Octet(const QString& value) : m_text(value) {};
        static bool is_valid(int value);
        bool is_valid() const;
        
    private:
        int int_value() const;

    private:
        const QString m_text;
        
    public:
        constexpr static const int min_value{0};
        constexpr static const int max_value{255};
    };


public:
    static QString from_int(const QString& int_text, bool* ok = nullptr);

public:
    IpV4(const QString& value) : m_text(value) {};
    bool is_valid() const;
    
private:
    const QString m_text;
    
public:
    constexpr static const int norm_separators_count {3};
    constexpr static const int norm_octets_count {norm_separators_count + 1};
    constexpr static const char octet_separator{'.'};
};

#endif // IPV4_H
