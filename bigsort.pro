TEMPLATE = app
CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += include \
    include/sorters \
    include/data_source_handlers

SOURCES += \
        main.cpp

HEADERS += \
    include/data_source_handlers/data_source.h \
    include/manager.h \
    include/sorters/isorter.h

DISTFILES += \
    .gitignore \
    CMakeLists.txt \
    README.md
