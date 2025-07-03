QT += core gui network websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = IUTFoodClient
TEMPLATE = app

SOURCES += \
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
    src/network/chatmanager.cpp \
    Client_UI/admin_signup.cpp \
    Client_UI/adminsetpass.cpp \
    Client_UI/cartitemwidget.cpp \
    Client_UI/cartwindow.cpp \
    Client_UI/customersetpass.cpp \
    Client_UI/custompopup.cpp \
    Client_UI/fooditemwidget.cpp \
    Client_UI/forgot_pass.cpp \
    Client_UI/history.cpp \
    Client_UI/homeadmin.cpp \
    Client_UI/main.cpp \
    Client_UI/menu.cpp \
    Client_UI/menuitemmanagerwidget.cpp \
    Client_UI/managemenuwindow.cpp \
    Client_UI/orderhistoryitem.cpp \
    Client_UI/orderitemwidget.cpp \
    Client_UI/profile.cpp \
    Client_UI/profileadmin.cpp \
    Client_UI/reset_pass.cpp \
    Client_UI/restaurantapprovalwindow.cpp \
    Client_UI/restaurantitemwidget.cpp \
    Client_UI/restaurantlistwindow.cpp \
    Client_UI/restaurantrequestitem.cpp \
    Client_UI/signup_coustomer.cpp \
    Client_UI/signup_restaurant.cpp \
    Client_UI/useritemwidget.cpp \
    Client_UI/usermanagementwindow.cpp \
    Client_UI/orderstatuswidget.cpp \
    Client_UI/homerestaurantowner.cpp \
    Client_UI/profilerestaurantowner.cpp \
    Client_UI/receivedorderswindow.cpp

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
    src/network/chatmanager.h \
    Client_UI/admin_signup.h \
    Client_UI/adminsetpass.h \
    Client_UI/cartitemwidget.h \
    Client_UI/cartwindow.h \
    Client_UI/customersetpass.h \
    Client_UI/custompopup.h \
    Client_UI/fooditemwidget.h \
    Client_UI/forgot_pass.h \
    Client_UI/history.h \
    Client_UI/homeadmin.h \
    Client_UI/homerestaurantowner.h \
    Client_UI/menu.h \
    Client_UI/managemenuwindow.h \
    Client_UI/menuitemmanagerwidget.h \
    Client_UI/orderhistoryitem.h \
    Client_UI/orderitemwidget.h \
    Client_UI/orderstatuswidget.h \
    Client_UI/profile.h \
    Client_UI/profileadmin.h \
    Client_UI/reset_pass.h \
    Client_UI/restaurantapprovalwindow.h \
    Client_UI/restaurantitemwidget.h \
    Client_UI/restaurantlistwindow.h \
    Client_UI/restaurantrequestitem.h \
    Client_UI/signup_coustomer.h \
    Client_UI/signup_restaurant.h \
    Client_UI/useritemwidget.h \
    Client_UI/usermanagementwindow.h \
    Client_UI/profilerestaurantowner.h \
    Client_UI/receivedorderswindow.h

FORMS += \
    Client_UI/admin_signup.ui \
    Client_UI/adminsetpass.ui \
    Client_UI/customersetpass.ui \
    Client_UI/forgot_pass.ui \
    Client_UI/homeadmin.ui \
    Client_UI/homerestaurantowner.ui \
    Client_UI/profile.ui \
    Client_UI/profileadmin.ui \
    Client_UI/profilerestaurantowner.ui \
    Client_UI/reset_pass.ui \
    Client_UI/signup_coustomer.ui \
    Client_UI/signup_restaurant.ui


RESOURCES += Client_UI/Source.qrc

INCLUDEPATH += src \
    Client_UI

# Compiler flags
QMAKE_CXXFLAGS += -Wall -Wextra

# Platform specific settings
win32 {
    LIBS += -lws2_32
}

unix {
    LIBS += -lssl -lcrypto
}