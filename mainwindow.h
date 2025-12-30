#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QVBoxLayout>

#include "testrunner.h"
#include "intipvalidator.h"

class MainWindow : public QWidget
{ Q_OBJECT
public:
    MainWindow() : QWidget() {
        {DirtyTestRunner();}

        auto main_lay = new QVBoxLayout(this);

        auto ip_edit = new QLineEdit(this);
        IntIpValidator::set_to(ip_edit);
        ip_edit->setText("0.0.0.0");

        main_lay->addWidget(ip_edit);

    }
};
#endif // MAINWINDOW_H
