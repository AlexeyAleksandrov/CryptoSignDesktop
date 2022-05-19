#include "mainwindow.h"

#include <QApplication>
#include <logger.h>

int main(int argc, char *argv[])
{
    Logger logger = Logger::getStartFinishObject(); // объект, который запишет в файл инфомацию о авершении работы программы

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
