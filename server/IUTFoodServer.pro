# QT += core network sql websockets
CONFIG += c++17 console
CONFIG -= app_bundle
QT += core websockets sql

TEMPLATE = app

SOURCES += \
    main.cpp \
    server.cpp \
    databasemanager.cpp \
    authsystem.cpp \
    invoker.cpp \
    command.cpp \
    ordercommand.cpp \
    observer.cpp \
    orderstatus.cpp \
    orderstatusobserver.cpp \
    securityutils.cpp \
    websocketserver.cpp \
    adminmanager.cpp \
    restaurantmanager.cpp \
    logger.cpp \
    customermanager.cpp \
    restaurantownermanager.cpp \
    ordermanager.cpp \
    menumanager.cpp

HEADERS += \
    server.h \
    databasemanager.h \
    authsystem.h \
    invoker.h \
    command.h \
    ordercommand.h \
    observer.h \
    orderstatus.h \
    orderstatusobserver.h \
    securityutils.h \
    websocketserver.h \
    adminmanager.h \
    restaurantmanager.h \
    logger.h \
    customermanager.h \
    restaurantownermanager.h \
    ordermanager.h \
    menumanager.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target 
