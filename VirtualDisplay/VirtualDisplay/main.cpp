#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#if defined(Q_OS_WIN32)
    QApplication::setStyle("fusion");
#endif
    MainWindow w;
    w.show();
    return a.exec();
}
