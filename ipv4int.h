#ifndef IPV4INT_H
#define IPV4INT_H

#include <QString>

class IpV4Int
{
public:
    IpV4Int(const QString& value);
    QString to_ipv4() const;
    
    bool is_valid() const;
    bool is_invalid() const;
    bool can_insert_first_separtor_to(const int inserted_index) const;
    
private:
    const QString m_text;
    
public:
    constexpr static const qint64 min_value{0x00000000};
    constexpr static const qint64 max_value{0xFFFFFFFF};
};

#endif // IPV4INT_H
