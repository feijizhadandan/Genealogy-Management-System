#include "mainwindow.h"

#include <QApplication>
#include "QRibbon/QRibbon.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QRibbon::install(&w);
    w.show();
    return a.exec();
}
