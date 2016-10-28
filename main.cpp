#include "mainwindow.h"
#include <QApplication>

#include <QImage>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //QImage image("C:\\Users\\dogea\\Desktop\\field.PNG");

    QImage image(":/img/record.PNG");

    QGraphicsPixmapItem item(QPixmap::fromImage(image));
    QGraphicsScene* scene = new QGraphicsScene;
    scene->addItem(&item);

    //QGraphicsView view(scene);
    //view.show();

    return a.exec();
}
