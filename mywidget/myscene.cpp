#include "myscene.h"

using namespace voronoiMap;

MyScene::MyScene()
{
//	qDebug() << typeid(*this).name() << "ctor";
}

MyScene::MyScene(const QSize& size):
	QGraphicsScene(0, 0, size.width(), size.height())
{
	vmap = new Voronoi(size.width(), size.height());
}

MyScene::~MyScene()
{
	delete vmap;
	//	qDebug() << typeid(*this).name() << "dtor";
}

//voronoiMap::Voronoi *MyScene::createVmap()
//{
//	delete vmap;
//	vmap = new Voronoi(this->width(), this->height());
//	int id = 0;
//	for(MyGraphicsEllipseItem* it : items){
//		Polygon *bar = new Polygon(it->centerPos().x(), it->centerPos().y());
//		bar->id = id++;
//		vmap->polygons.push_back(bar);
//	}
//	return vmap;
//}

void MyScene::setVmap()
{
	delete vmap;
	vmap = new Voronoi(this->width(), this->height());
	this->syncVmap();
}

void MyScene::setVmap(Voronoi *vmap)
{
	delete vmap;
	this->vmap = vmap;
	this->syncVmap();
}

void MyScene::clearContent()
{
	delete vmap;
	vmap = nullptr;
	this->clear();
	ellipseItems.clear();
	lineItems.clear();
}

void MyScene::syncVmap()
{
	this->clearContent();
	this->setSceneRect(0, 0, vmap->width, vmap->height);
	this->addRect(this->sceneRect(), QPen(Qt::white), QBrush(Qt::white))->setZValue(-100);
	for(auto poly:vmap->polygons){
		MyGraphicsEllipseItem *item = new MyGraphicsEllipseItem(poly->focus->x, poly->focus->y, 5, 5);
		item->setPos(item->pos());
		this->addItem(item);
		ellipseItems.push_back(std::shared_ptr<MyGraphicsEllipseItem>(item));
		for(auto edge:poly->edges){
			// wololo
		}
	}
}


void MyScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	event->acceptProposedAction();
}

void MyScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
	event->acceptProposedAction();
}

void MyScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
}

QPointF MousePrevPoint;
MyGraphicsEllipseItem *obj;

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	auto item = this->itemAt(event->scenePos(), QTransform());
	if(dynamic_cast<MyGraphicsEllipseItem*>(item) != nullptr){
		MousePrevPoint = event->scenePos();
		obj = dynamic_cast<MyGraphicsEllipseItem*>(item);
	}
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(obj != nullptr){
		QPointF newPos = obj->centerPos() + (event->scenePos() - MousePrevPoint);
		if(this->sceneRect().contains(newPos)){
			obj->setCenterPos(newPos.x(), newPos.y());
		}
		MousePrevPoint = event->scenePos();
	}
//	createVmap();
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	obj = nullptr;
	MousePrevPoint.setX(0);
	MousePrevPoint.setY(0);
//	createVmap();
}

void MyScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	QPointF point = event->scenePos();
	MyGraphicsEllipseItem *item = new MyGraphicsEllipseItem(point.x(), point.y(), 5, 5);
	item->setPos(item->pos());
	this->addItem(item);
	ellipseItems.push_back(std::shared_ptr<MyGraphicsEllipseItem>(item));
//	createVmap();
}
