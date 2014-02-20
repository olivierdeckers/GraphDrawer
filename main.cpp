#include "mainwindow.h"
#include "accstructcomparison.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //AccStructComparison w;
    w.show();

    return a.exec();
}
