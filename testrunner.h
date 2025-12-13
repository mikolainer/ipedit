#ifndef TESTRUNNER_H
#define TESTRUNNER_H

#include "test_ip_edit.h"
#include "test_oneclick_testfactory.h"

class DirtyTestRunner
{
public:
    DirtyTestRunner()
    {
        run_all();
    }

private:
    void run_all()
    {
        {
            auto test = Test_OneClickHelpers();
            QTest::qExec(&test);
        }
        {
            auto test = Test_IpEdit();
            QTest::qExec(&test);
        }
    }
};

#endif // TESTRUNNER_H
