#ifndef TEST_ONECLICK_TESTFACTORY_H
#define TEST_ONECLICK_TESTFACTORY_H

#include "oneclick_testfactory.h"

class Test_OneClickHelpers : public QObject
{ Q_OBJECT

    const State state{"12.12.12.12", 2};
public:
    Test_OneClickHelpers(QObject* parent = nullptr);

private slots:
    void test_make_click_ch_space();
    void test_make_click_ch_dot();
    void test_make_click_ch_0();
    void test_make_click_ch_1();
    void test_make_click_ch_2();
    void test_make_click_ch_3();
    void test_make_click_ch_4();
    void test_make_click_ch_5();
    void test_make_click_ch_6();
    void test_make_click_ch_7();
    void test_make_click_ch_8();
    void test_make_click_ch_9();
    void test_make_click_ch_comma();
    void test_make_click_key_space();
    void test_make_click_key_0();
    void test_make_click_key_1();
    void test_make_click_key_2();
    void test_make_click_key_3();
    void test_make_click_key_4();
    void test_make_click_key_5();
    void test_make_click_key_6();
    void test_make_click_key_7();
    void test_make_click_key_8();
    void test_make_click_key_9();
    void test_make_input_set();
    void test_make_valid_cases_insert();
    void test_make_valid_cases_remove_forward();
    void test_make_valid_cases_remove_backward();
    void test_make_invalid_cases_insert();
    void test_make_moveonly_cases_insert();
    void test_make_moveonly_cases_remove_forward();
    void test_make_moveonly_cases_remove_backward();
    void test_make_insertover_cases();
    void test_make_replacebefore_cases();
    void test_make_test_name_insert_ch();
    void test_make_test_name_insert_key();
    void test_make_test_name_remove_forward();
    void test_make_test_name_remove_backward();
};

#endif // TEST_ONECLICK_TESTFACTORY_H
