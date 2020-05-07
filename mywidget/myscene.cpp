#include "myscene.h"

using namespace voronoiMap;

MyScene::MyScene()
{
//	qDebug() << typeid(*this).name() << "ctor";
}

MyScene::MyScene(const QSize& size):
	QGraphicsScene(0, 0, size.width(), size.height())
{
	voronoiGen.setVmap(new Voronoi(size.width(), size.height()));
}

MyScene::~MyScene()
{
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
	voronoiGen.setVmap(new Voronoi(this->width(), this->height()));
	this->syncVmap();
}

void MyScene::setVmap(Voronoi *vmap)
{
	voronoiGen.setVmap(vmap);
	this->syncVmap();
}

void MyScene::clearContent()
{
	ellipseItems.clear();
	lineItems.clear();
	this->clear();
}

void MyScene::syncVmap()
{
	const Voronoi *vmap = voronoiGen.vmap;
	ellipseItems.clear();
	lineItems.clear();
	this->clear();
	this->setSceneRect(0, 0, vmap->width, vmap->height);
	this->addRect(this->sceneRect(), QPen(Qt::white), QBrush(Qt::white))->setZValue(-100);
	for(const auto& poly:vmap->polygons){
		MyGraphicsEllipseItem *item = new MyGraphicsEllipseItem(poly->focus->x, poly->focus->y, 5, 5);
		item->setPos(item->pos());
		this->addItem(item);
		ellipseItems.push_back(std::shared_ptr<MyGraphicsEllipseItem>(item));
		for(auto edge:poly->edges){
			if(edge->a == nullptr || edge->b == nullptr) continue;
			QGraphicsLineItem* litem = new QGraphicsLineItem(edge->a->x, edge->a->y, edge->b->x, edge->b->y);
			this->addItem(litem);
			lineItems.push_back(std::shared_ptr<QGraphicsLineItem>(litem));
		}
	}
}

void MyScene::syncFortune(){
	const SweepLine *sweepLine = voronoiGen.sweepLine;
	if(sweepLine == nullptr) return;
	this->syncVmap();
	QGraphicsLineItem* litem = new QGraphicsLineItem(sweepLine->L, 0, sweepLine->L, voronoiGen.vmap->height);
	this->addItem(litem);
	lineItems.push_back(std::shared_ptr<QGraphicsLineItem>(litem));
}

void MyScene::setAutoFortune(bool action)
{
	autoFortune = action;
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
Polygon* pobj;

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	Voronoi *vmap = voronoiGen.vmap;
	auto item = this->itemAt(event->scenePos(), QTransform());
	if(dynamic_cast<MyGraphicsEllipseItem*>(item) != nullptr){
		MousePrevPoint = event->scenePos();
		obj = dynamic_cast<MyGraphicsEllipseItem*>(item);
		auto it = std::find_if(vmap->polygons.begin(), vmap->polygons.end(), [&](Polygon* a){
				return abs(a->focus->x - (int)item->x()) < 5 && abs(a->focus->y - item->y()) < 5;
		});
		if(it != vmap->polygons.end()){
			pobj = *it;
		}
	}
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(obj != nullptr){
		QPointF newPos = obj->centerPos() + (event->scenePos() - MousePrevPoint);
		if(this->sceneRect().contains(newPos)){
			obj->setCenterPos(newPos.x(), newPos.y());
			if(pobj){
				pobj->focus->x = newPos.x();
				pobj->focus->y = newPos.y();
			}
		}
		MousePrevPoint = event->scenePos();
	}
	if(autoFortune)
		voronoiGen.performFortune();
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
	MyGraphicsEllipseItem *item = new MyGraphicsEllipseItem(point.x(), point.y(), 5, 5);
	item->setPos(item->pos());
	this->addItem(item);
	ellipseItems.push_back(std::shared_ptr<MyGraphicsEllipseItem>(item));
	voronoiGen.vmap->addPoly(new Polygon(point.x(), point.y()));
	if(autoFortune)
		voronoiGen.performFortune();
}
