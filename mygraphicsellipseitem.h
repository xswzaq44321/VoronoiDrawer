#ifndef MYGRAPHICSELLIPSEITEM_H
#define MYGRAPHICSELLIPSEITEM_H

#include <QGraphicsEllipseItem>
#include <QRectF>
#include <QWidget>

#include "voronoimap.h"

class MyGraphicsEllipseItem : public QGraphicsEllipseItem
{
public:
	MyGraphicsEllipseItem();
	MyGraphicsEllipseItem(double x, double y, double width = 5, double height = 5);

	voronoiMap::Point *point = nullptr;

	void setPos(const QPointF &pos);
	void setSize(const QSize &size);
};

#endif // MYGRAPHICSELLIPSEITEM_H
