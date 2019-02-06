#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowIcon(QIcon(":/img/brazil_64.png"));
    w.setWindowTitle("Separador de UFs");

    w.show();

    return a.exec();
}
