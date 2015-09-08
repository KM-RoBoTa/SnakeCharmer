#include <QApplication>
#include <iostream>

#include "qtgui/controller.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Controller *w = new Controller();
    w->show();
    a.exec();
    cout << "End" << endl;
    return 0;
}
