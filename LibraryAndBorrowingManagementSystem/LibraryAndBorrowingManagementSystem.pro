QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bookview.cpp \
    borrowandreturndataview.cpp \
    borrowandreturnview.cpp \
    borrowview.cpp \
    dataview.cpp \
    idatabase.cpp \
    loginview.cpp \
    main.cpp \
    managementview.cpp \
    masterview.cpp \
    personalinformationview.cpp \
    readerborrowandreturnview.cpp \
    readerview.cpp \
    registerview.cpp \
    returnview.cpp

HEADERS += \
    bookview.h \
    borrowandreturndataview.h \
    borrowandreturnview.h \
    borrowview.h \
    dataview.h \
    idatabase.h \
    loginview.h \
    managementview.h \
    masterview.h \
    personalinformationview.h \
    readerborrowandreturnview.h \
    readerview.h \
    registerview.h \
    returnview.h

FORMS += \
    bookview.ui \
    borrowandreturndataview.ui \
    borrowandreturnview.ui \
    borrowview.ui \
    dataview.ui \
    loginview.ui \
    managementview.ui \
    masterview.ui \
    personalinformationview.ui \
    readerborrowandreturnview.ui \
    readerview.ui \
    registerview.ui \
    returnview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
