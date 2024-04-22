#include "mainwindow.h"
#include "Initial.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    Turn_on_Log(1, 0);
    LOG_TRACE("Turn_on_Log!!");
//    LOG_DEBUG("Testing debug macro");
//    LOG_INFO("Testing inf macro");
//    LOG_WARN("Testing warn macro");
//    LOG_FATAL("Testing crit macro2");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
