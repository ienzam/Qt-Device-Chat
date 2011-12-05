#include <QtGui/QApplication>
#include "serialconnectwindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("BUET");
    QCoreApplication::setOrganizationDomain("buet.ac.bd");
    QCoreApplication::setApplicationName("Generic Send Recieve");

    QApplication a(argc, argv);
    ConnectWindow w;
    w.show();
    return a.exec();
}
