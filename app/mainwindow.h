#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include "intipedit.h"

class MainWindow : public QWidget
{ Q_OBJECT
public:
    MainWindow() : QWidget() {
        auto main_lay = new QVBoxLayout(this);
        main_lay->addWidget(new IntIpEdit(this));
    }
};
