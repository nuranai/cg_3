#include "mainwindow.h"

#include <QApplication>
#include <win.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    win w;
    w.show();
    return a.exec();
}
