TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c \
    ../../nowaitlock.c

INCLUDEPATH += ../..

HEADERS += \
    ../../nowaitlock.h
