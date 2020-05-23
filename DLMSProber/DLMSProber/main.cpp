#include "prober.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Prober w;
    w.show();

    return a.exec();
}
