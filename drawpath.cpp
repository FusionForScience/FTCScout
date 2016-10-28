#include "drawpath.h"
#include <QDebug>

DrawPath::DrawPath(QObject *parent) :
    QGraphicsScene(parent)
{
    drawField();
    hasDrawn = false;
    allowDraw = true;
}

DrawPath::~DrawPath()
{

}

void DrawPath::drawField()
{
    QImage image(":/img/Resources/img/field.PNG");

    QPixmap item(QPixmap::fromImage(image));

    this->addPixmap(item);
}

void DrawPath::printPath()
{
    for(int i = 0; i < path.size(); i ++)
    {
        qDebug() << "Point- x: " << path[i].x() << " y: " << path[i].y() << endl;
    }
}

vector<QPointF> DrawPath::getPath()
{
    return path;
}

void DrawPath::drawPath(vector<QPointF> path)
{
    for(int i = 0; i < path.size(); i ++)
    {
        drawPoint(path[i]);
    }
}

void DrawPath::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->buttons() && Qt::LeftButton && allowDraw)
    {
        hasDrawn = true;
        QPointF pt = mouseEvent->scenePos();
        addToPath(pt);
        drawPoint(pt);
        emit userHasDrawn();
    }
}

void DrawPath::addToPath(QPointF myPoint)
{
    path.push_back(myPoint);
    return;
}

void DrawPath::drawPoint(QPointF pt)
{
    this->addEllipse(pt.x(), pt.y(), drawRadius * 2.0, drawRadius * 2.0,QPen(), QBrush(Qt::SolidPattern));
    return;
}
