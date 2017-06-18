#include "speedenter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
     QApplication a(argc, argv);
     SpeedEnter s;
     s.show();
     return a.exec();
}
