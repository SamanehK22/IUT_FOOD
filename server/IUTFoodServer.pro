# QT += core network sql websockets
CONFIG += c++17 console
CONFIG -= app_bundle
QT += core websockets sql

TEMPLATE = app

SOURCES += \
    main.cpp \
    databasemanager.cpp \
    logger.cpp \
    securityutils.cpp \
    authsystem.cpp \
    command.cpp \
    invoker.cpp \
    ordercommand.cpp \
    observer.cpp \
    orderstatusobserver.cpp \
    orderstatus.cpp

HEADERS += \
    databasemanager.h \
    logger.h \
    securityutils.h \
    authsystem.h \
    command.h \
    invoker.h \
    ordercommand.h \
    observer.h \
    orderstatusobserver.h \
    orderstatus.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target 
