#-------------------------------------------------
#
# Project created by QtCreator 2017-05-21T00:48:29
#
#-------------------------------------------------

#To do
#1  - розібратись з роботою кріейтора, редактор питань обмежети тільки на редагування
#а створення тесту вже має всі ф-кції

#2 - зібрати NTest вже готову, і зробити інсталяшку


QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql printsupport


TARGET = NTest-Creator
TEMPLATE = app

RC_FILE = myapp.rc


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    speedenter.cpp \
    students.cpp \
    result.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    speedenter.h \
    students.h \
    result.h \
    about.h

FORMS    += mainwindow.ui \
    speedenter.ui \
    students.ui \
    result.ui \
    about.ui

DISTFILES += \
    myapp.rc
