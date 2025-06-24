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
    authsystem.cpp 

HEADERS += \
    databasemanager.h \
    logger.h \
    securityutils.h \
    authsystem.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target 
