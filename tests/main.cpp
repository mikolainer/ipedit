#include "testrunner.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TestRunner::run_all();
}
