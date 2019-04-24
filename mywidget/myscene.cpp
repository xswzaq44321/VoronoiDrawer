#include "myscene.h"

using namespace voronoiMap;

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

voronoiMap::Voronoi *MyScene::createVmap()
{
	delete vmap;
	vmap = new Voronoi(this->width(), this->height());
	for(MyGraphicsEllipseItem* it : items){
		Polygon *bar = new Polygon(it->x(), it->y());
		vmap->polygons.push_back(bar);
		qDebug() << it->x() << ' ' << it->y();
	}
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
	createVmap();
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	obj = nullptr;
	MousePrevPoint.setX(0);
	MousePrevPoint.setY(0);
	createVmap();
}

void MyScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF point = event->scenePos();
	MyGraphicsEllipseItem *item = new MyGraphicsEllipseItem(point.x(), point.y(), 5, 5);
	item->setPos(point);
	this->addItem(item);
	items.push_back(item);
	createVmap();
}
