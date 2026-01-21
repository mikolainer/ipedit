TEMPLATE = lib

QT += core
QT += gui
QT += widgets

CONFIG += c++17

DEFINES += IS_IP_EDIT_LIB

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    intipedit.cpp \
    intipv4calculator.cpp \
    intipvalidator.cpp \
    ipv4.cpp \
    ipv4int.cpp \
    ipv4octet.cpp \
    manualdiff.cpp \
    textchangehistory.cpp \
    texteditstate.cpp

HEADERS += \
    global.h \
    intipedit.h \
    intipv4calculator.h \
    intipvalidator.h \
    ipv4.h \
    ipv4int.h \
    manualdiff.h \
    textchangehistory.h \
    texteditstate.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
