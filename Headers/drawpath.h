#ifndef DRAWPATH_H
#define DRAWPATH_H

#include <QGraphicsScene>
#include <QPoint>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

#include <iostream>
#include <vector>
#include <fstream>

#include "team.h"

using namespace std;

class DrawPath : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit DrawPath(QObject *parent = 0);
    ~DrawPath();

    bool hasDrawn;
    bool allowDraw;

    void drawField();
    void printPath();
    vector <QPointF> getPath();
    void drawPath(vector <QPointF> path);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent);

private:
    int drawRadius = 3;

    vector <QPointF> path;

    void addToPath(QPointF myPoint);
    void drawPoint(QPointF pt);

signals:
    void userHasDrawn();
};

#endif // DRAWPATH_H
