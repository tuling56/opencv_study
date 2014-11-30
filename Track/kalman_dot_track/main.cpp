#include <QApplication>
#include "kalman.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    kalman w;
    w.show();
    
    return a.exec();
}
