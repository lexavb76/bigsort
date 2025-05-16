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
    include/data_source_handlers/data_chunk.h \
    include/data_source_handlers/data_source_base.h \
    include/data_source_handlers/file_data_source.h \
    include/data_source_handlers/iterator_switch.h \
    include/manager.h \
    include/sorters/imerger.h \
    include/sorters/isorter.h \
    include/types.h

DISTFILES += \
    .gitignore \
    CMakeLists.txt \
    README.md
