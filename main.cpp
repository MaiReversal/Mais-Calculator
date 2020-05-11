#include "mainwindow.h"
#include <QApplication>
#include <conio.h>

int main(int argc, char *argv[])
{
        QApplication a(argc, argv);
        MainWindow w;
        w.setWindowTitle("Mai's calculator");
        w.show();
    return a.exec();
}
