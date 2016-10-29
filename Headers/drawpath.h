#ifndef DRAWPATH_H
#define DRAWPATH_H

#include <QGraphicsScene>
#include <QPoint>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

#include <vector>

#include "team.h"

using namespace std;

class DrawPath : public QGraphicsScene
{
    Q_OBJECT
public:
    // Constructors/Destructors
    explicit DrawPath(QObject *parent = 0);
    ~DrawPath();

    bool hasDrawn;
    bool allowDraw;

    void drawField();
    void printPath();
    vector <QPointF> getPath();
    void drawPath(vector <QPointF> path);

private:
    const int drawRadius = 3;

    vector <QPointF> path;

    // Function prototypes
    void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent);
    void addToPath(QPointF myPoint);
    void drawPoint(QPointF pt);

signals:
    void userHasDrawn();
};

#endif // DRAWPATH_H
