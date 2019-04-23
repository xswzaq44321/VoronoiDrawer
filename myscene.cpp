#include "myscene.h"

MyScene::MyScene()
{
//	qDebug() << typeid(*this).name() << "ctor";
}

MyScene::MyScene(const QSize& size):
	QGraphicsScene(0, 0, size.width(), size.height())
{
}

MyScene::~MyScene()
{
//	qDebug() << typeid(*this).name() << "dtor";
}


void MyScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
}

void MyScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
}

void MyScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
}

QPointF MousePrevPoint;
QGraphicsItem *obj;

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	auto item = this->itemAt(event->scenePos(), QTransform());
	if(dynamic_cast<QGraphicsEllipseItem*>(item) != nullptr){
		MousePrevPoint = event->scenePos();
		obj = item;
	}
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(obj != nullptr){
		obj->setPos(obj->pos() + (event->scenePos() - MousePrevPoint));
		MousePrevPoint = event->scenePos();
	}
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	obj = nullptr;
	MousePrevPoint.setX(0);
	MousePrevPoint.setY(0);
}

void MyScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF point = event->scenePos();
	vmap->addPoly(point.x(), point.y());
	MyGraphicsEllipseItem *item = new MyGraphicsEllipseItem(point.x(), point.y());
	this->addItem(item);
}
