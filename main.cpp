#include "mainwindow.h"
#include "speedenter.h"
#include "students.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    SpeedEnter s;
    Students st;
    s.show();

    QObject::connect(&s, SIGNAL(editTest()),&w, SLOT(OpenDB()));
    QObject::connect(&s, SIGNAL(createTest()),&w, SLOT(SaveAsDB()));

    QObject::connect(&w, SIGNAL(openList(QString)), &st, SLOT(Open(QString)));
    QObject::connect(&w, SIGNAL(createList()), &st, SLOT(show()));

    QObject::connect(&s, SIGNAL(students()), &st, SLOT(show()));

    return a.exec();
}
