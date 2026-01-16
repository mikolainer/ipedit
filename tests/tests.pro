TEMPLATE = app
QT += testlib
QT += widgets
QT += gui

CONFIG += c++17
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

LIBS += -LC:\Users\mikol\Pet\ipedit\build\Desktop_Qt_6_10_0_MinGW_64_bit-Debug\debug -lipedit
INCLUDEPATH += ..

SOURCES +=  \
    main.cpp \
    oneclick_testfactory.cpp \
    test_ip_edit.cpp \
    test_oneclick_testfactory.cpp

HEADERS += \
    oneclick_testfactory.h \
    test_ip_edit.h \
    test_oneclick_testfactory.h \
    testrunner.h
