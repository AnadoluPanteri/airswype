#-------------------------------------------------
#
# Project created by QtCreator 2011-05-18T10:40:01
#
#-------------------------------------------------

QT       += core gui


TARGET = qtmatchtest
TEMPLATE = app
INCLUDEPATH += ../../src


SOURCES += main.cpp\
        dialog.cpp \
    ../../src/formanalyzer.cpp

HEADERS  += dialog.h \
    ../../src/datatypes.h \
    ../../src/formanalyzer.h

FORMS    += dialog.ui
