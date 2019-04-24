#ifndef MYGRAPHICSELLIPSEITEM_H
#define MYGRAPHICSELLIPSEITEM_H

#include <QGraphicsEllipseItem>
#include <QRectF>
#include <QWidget>

#include "voronoi/voronoimap.h"

class MyGraphicsEllipseItem : public QGraphicsEllipseItem
{
public:
	MyGraphicsEllipseItem();
	MyGraphicsEllipseItem(double x, double y, double width = 5, double height = 5);

	void setPos(const QPointF &pos);
	void setSize(const QSize &size);
};

#endif // MYGRAPHICSELLIPSEITEM_H
