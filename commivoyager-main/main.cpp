#include "mainwindow.h"
#include "top.h"
#include "edge.h"
#include <QApplication>
#include <QLineEdit>
#include <QSplashScreen>
#include <QTimer>
QString status="none";
int len=0;
int mem_x;
int mem_y;
int mem_num;
QVector <TOP> graph1;
QVector <EDGE> graph2;
QVector <QVector<int>> matrix;
QString statusBook[]={"top","edge1.1","edge2.1","move","del","none"};
QString text_for_arrows;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("C:/Users/Lexa/Downloads/commivoyager-main/commivoyager-main/earth.ico"));

    QPixmap pixmap("C:/Users/Lexa/Downloads/commivoyager-main/commivoyager-main/cosmo.png");
    QSplashScreen splash(pixmap);
    splash.setMask(pixmap.mask());
    splash.show();

    MainWindow w;

    QTimer::singleShot(3000, &splash, SLOT(close()));
    QTimer::singleShot(3000, &w, SLOT(show()));
    //w.show();
    return a.exec();
}
