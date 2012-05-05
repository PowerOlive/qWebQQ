#include <QtGui/QApplication>
#include "widget.h"
#include "util.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Labo-A.L");
    a.setApplicationName("InsecureQQ");

    Util::init();

    Widget w;
    w.show();
    
    return a.exec();
}
