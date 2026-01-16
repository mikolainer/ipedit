#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include "intipvalidator.h"

class MainWindow : public QWidget
{ Q_OBJECT
public:
    MainWindow() : QWidget() {
        auto main_lay = new QVBoxLayout(this);

        auto ip_edit = new QLineEdit(this);
        IntIpValidator::set_to(ip_edit);
        ip_edit->setText("0.0.0.0");

        main_lay->addWidget(ip_edit);

    }
};
