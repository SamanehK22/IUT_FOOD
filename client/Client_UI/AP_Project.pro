QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    admin_signup.cpp \
    adminsetpass.cpp \
    cartitemwidget.cpp \
    cartwindow.cpp \
    customersetpass.cpp \
    custompopup.cpp \
    fooditemwidget.cpp \
    forgot_pass.cpp \
    history.cpp \
    homeadmin.cpp \
    main.cpp \
    menu.cpp \
    orderhistoryitem.cpp \
    profile.cpp \
    profileadmin.cpp \
    reset_pass.cpp \
    restaurantapprovalwindow.cpp \
    restaurantitemwidget.cpp \
    restaurantlistwindow.cpp \
    restaurantrequestitem.cpp \
    signup_coustomer.cpp \
    signup_restaurant.cpp \
    useritemwidget.cpp \
    usermanagementwindow.cpp

HEADERS += \
    admin_signup.h \
    adminsetpass.h \
    cartitemwidget.h \
    cartwindow.h \
    customersetpass.h \
    custompopup.h \
    fooditemwidget.h \
    forgot_pass.h \
    history.h \
    homeadmin.h \
    menu.h \
    orderhistoryitem.h \
    profile.h \
    profileadmin.h \
    reset_pass.h \
    restaurantapprovalwindow.h \
    restaurantitemwidget.h \
    restaurantlistwindow.h \
    restaurantrequestitem.h \
    signup_coustomer.h \
    signup_restaurant.h \
    useritemwidget.h \
    usermanagementwindow.h

FORMS += \
    admin_signup.ui \
    adminsetpass.ui \
    customersetpass.ui \
    forgot_pass.ui \
    homeadmin.ui \
    profile.ui \
    profileadmin.ui \
    reset_pass.ui \
    signup_coustomer.ui \
    signup_restaurant.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Source.qrc
