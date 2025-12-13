#ifndef TEST_IP_EDIT_H
#define TEST_IP_EDIT_H

#include<QObject>

class Test_IpEdit : public QObject
{ Q_OBJECT
    constexpr static const Qt::Key backspace_key = Qt::Key_Backspace;
    constexpr static const Qt::Key delete_key = Qt::Key_Delete;
    constexpr static const Qt::Key rarrow_key = Qt::Key_Right;
    constexpr static const Qt::Key larrow_key = Qt::Key_Left;

public:
    Test_IpEdit(QObject* parent = nullptr);

private slots:
    void oneClick_data();
    void oneClick();

    void clear_all_by_backspace_0_0_0_0();
    void clear_all_by_delete_0_0_0_0();
};

#endif // TEST_IP_EDIT_H
