#include "mygraphicsellipseitem.h"

#include <QDebug>

MyGraphicsEllipseItem::MyGraphicsEllipseItem()
{
}

MyGraphicsEllipseItem::MyGraphicsEllipseItem(double x, double y, double width, double height):
	QGraphicsEllipseItem(x - width / 2,y - height / 2,width,height)
{
	this->setBrush(Qt::black);
	this->setPos(x - width / 2,y - height / 2);
}

void MyGraphicsEllipseItem::setPos(const QPointF &pos)
{
	QGraphicsEllipseItem::setPos(pos);
	double x = pos.x() - this->rect().width() / 2;
	double y = pos.y() - this->rect().height() / 2;
}

void MyGraphicsEllipseItem::setSize(const QSize &size)
{
	double width = size.width();
	double height = size.height();
	double x = this->x() - width / 2;
	double y = this->y() - height / 2;
	this->setRect(x, y, width, height);
}
