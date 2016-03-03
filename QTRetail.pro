#-------------------------------------------------
#
# Project created by QtCreator 2016-02-24T15:28:45
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTRetail
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    stock.cpp \
    main.cpp \
    sqlite3.c

HEADERS  += mainwindow.h \
    stock.h \
    sqlite3.h


FORMS    += mainwindow.ui \
    stock.ui

LIBS += -lpthread
LIBS += -ldl

