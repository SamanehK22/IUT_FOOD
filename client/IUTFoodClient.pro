QT += core gui network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = IUTFoodClient
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/models/user.cpp \
    src/models/restaurant.cpp \
    src/models/menuitem.cpp \
    src/models/order.cpp \
    src/models/admin.cpp \
    src/models/customer.cpp \
    src/models/restaurantowner.cpp \
    src/models/cart.cpp \
    src/models/chatmessage.cpp \
    src/network/networkmanager.cpp \
    src/network/authmanager.cpp \
    src/network/restaurantmanager.cpp \
    src/network/ordermanager.cpp \
    src/network/chatmanager.cpp

HEADERS += \
    src/models/user.h \
    src/models/restaurant.h \
    src/models/menuitem.h \
    src/models/order.h \
    src/models/admin.h \
    src/models/customer.h \
    src/models/restaurantowner.h \
    src/models/cart.h \
    src/models/chatmessage.h \
    src/network/networkmanager.h \
    src/network/authmanager.h \
    src/network/restaurantmanager.h \
    src/network/ordermanager.h \
    src/network/chatmanager.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Include paths
INCLUDEPATH += src/

# Compiler flags
QMAKE_CXXFLAGS += -Wall -Wextra

# Platform specific settings
win32 {
    LIBS += -lws2_32
}

unix {
    LIBS += -lssl -lcrypto
} 