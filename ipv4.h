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
        bool is_prevalid() const;
        
    private:
        int int_value() const;

    private:
        const QString m_text;
        
    public:
        constexpr static const int min_value{0};
        constexpr static const int max_value{255};

        constexpr static const char* availavle_chars {"1234567890"};
    };


public:
    static QString fix(const QString& ip_text);

public:
    IpV4(const QString& value) : m_text(value) {};
    bool is_valid() const;
    bool is_prevalid() const;
    
private:
    const QString m_text;
    
public:
    constexpr static const int norm_separators_count {3};
    constexpr static const int norm_octets_count {norm_separators_count + 1};
    constexpr static const char octet_separator{'.'};
    constexpr static const char* wrong_octet_separators{" .,/?ю"};
};

#endif // IPV4_H
