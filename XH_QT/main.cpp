#include "xh_test.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XH_Test w;
    w.show();

    return a.exec();
}
