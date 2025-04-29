TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += include \
    include/sorters

SOURCES += \
        main.cpp

HEADERS += \
    include/manager.h \
    include/sorters/isorter.h
