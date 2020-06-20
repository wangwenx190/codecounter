#include "frmcountcode.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication application(argc, argv);

    frmCountCode window;
    window.show();

    return QCoreApplication::exec();
}
