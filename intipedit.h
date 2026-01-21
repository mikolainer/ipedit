#pragma once

#include "global.h"
#include <QLineEdit>

class LIB_EXPORT IntIpEdit : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString ip READ getIp WRITE setIp NOTIFY onIpChanged)

public:
    explicit IntIpEdit(QWidget *parent = nullptr);
    explicit IntIpEdit(const QString& text, QWidget *parent = nullptr);

public:
    QString getIp() const;
public slots:
    void setIp(const QString& text);
signals:
    void onIpChanged(const QString& text);

private slots:
    void ipChanged(const QString& text);
private:
    void setup_ui();

private:
    QLineEdit m_edit;
    static constexpr const char* default_value = "0.0.0.0";
};
