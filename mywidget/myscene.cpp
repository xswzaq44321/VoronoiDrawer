#include "myscene.h"

using namespace voronoiMap;

MyScene::MyScene(const QSize& size):
	QGraphicsScene(0, 0, size.width(), size.height())
{
	this->setVmap(new Voronoi(size.width(), size.height()));
	initialProgression();
}

MyScene::MyScene(Voronoi *vmap)
{
	this->setVmap(vmap);
	initialProgression();
}

MyScene::~MyScene()
{
	//	qDebug() << typeid(*this).name() << "dtor";
}

//void MyScene::setVmap()
//{
//	voronoiGen.setVmap(new Voronoi(this->width(), this->height()));
//	this->syncVmap();
//	mapCanvas = new QPixmap(this->width(), this->height());
//}

void MyScene::setVmap(Voronoi *vmap)
{
	mapCanvas = new QPixmap(vmap->width, vmap->height);
	mapCanvas->fill();
	voronoiGen.setVmap(vmap);
	this->setSceneRect(0, 0, vmap->width, vmap->height);
	mapCanvasItem = this->addPixmap(*mapCanvas);
	mapCanvasItem->setZValue(-10000);
	this->syncVmap();
}

void MyScene::clearContent()
{
	ellipseItems.clear();
	lineItems.clear();
	this->clear();
}

void MyScene::syncVmap(bool updatePoint)
{
	const Voronoi *vmap = voronoiGen.vmap;
	if(updatePoint){
		for (std::shared_ptr<MyGraphicsEllipseItem> it : ellipseItems) {
			this->removeItem(it.get());
		}
		ellipseItems.clear();
	}
	for (std::shared_ptr<QGraphicsLineItem> it : lineItems) {
		this->removeItem(it.get());
	}
	lineItems.clear();
	for(const auto& poly:vmap->polygons){
		if(updatePoint){
			MyGraphicsEllipseItem *item = new MyGraphicsEllipseItem(poly->focus->x, poly->focus->y, 5, 5);
			item->setPos(item->pos());
			this->addItem(item);
			ellipseItems.push_back(std::shared_ptr<MyGraphicsEllipseItem>(item));
		}
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

void MyScene::drawTerrain(float altitudeMax)
{
	mapCanvas->fill();

	QPainter painter(mapCanvas);
	const auto& pointMap = voronoiGen.pointMap;
	for (int x = 0; x < pointMap.size(); ++x) {
		for (int y = 0; y < pointMap.at(x).size(); ++y) {
			auto altitude = pointMap.at(x).at(y).terrain.altitude;
			double gradient = altitude / altitudeMax;
			gradient = std::max(0.0, std::min(1.0, gradient));
			QColor color = terrProg.pixel(0, (1 - gradient) * (terrProg.height() - 1));
			painter.setPen(color);
			painter.setBrush(color);
			painter.drawPoint(x, y);
		}
	}

	this->addPixmap(QPixmap::fromImage(terrProg))
			->setPos(this->width() - terrProg.width(), this->height() - terrProg.height());
	this->addRect(0, 0, terrProg.width(), terrProg.height(), QPen(Qt::black), QBrush(Qt::transparent))
			->setPos(this->width() - terrProg.width(), this->height() - terrProg.height());
	mapCanvasItem->setPixmap(*mapCanvas);
}

void MyScene::drawWater()
{
	QPainter painter(mapCanvas);
	const auto& pointMap = voronoiGen.pointMap;
	for (int x = 0; x < pointMap.size(); ++x) {
		for (int y = 0; y < pointMap.at(x).size(); ++y) {
			if(pointMap[x][y].terrain.type == "river" || pointMap[x][y].terrain.type == "water"){
				QColor color(0, 153, 255);
				painter.setPen(color);
				painter.setBrush(color);
				painter.drawPoint(x, y);
			}
		}
	}
	mapCanvasItem->setPixmap(*mapCanvas);
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
	if(autoFortune){
		voronoiGen.performFortune();
		this->syncVmap(false);
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
	MyGraphicsEllipseItem *item = new MyGraphicsEllipseItem(point.x(), point.y(), 5, 5);
	item->setPos(item->pos());
	this->addItem(item);
	ellipseItems.push_back(std::shared_ptr<MyGraphicsEllipseItem>(item));
	voronoiGen.vmap->addPoly(new Polygon(point.x(), point.y()));
	if(autoFortune){
		voronoiGen.performFortune();
		this->syncVmap();
	}
}

void MyScene::initialProgression()
{
	QPixmap* canvas = new QPixmap(20, 150);
	QPainter painter(canvas);
	QLinearGradient linear(0, 0, 0, canvas->height());
	linear.setColorAt(0, QColor::fromRgb(102, 51, 0));
	linear.setColorAt(0.5, QColor::fromRgb(255, 255, 51));
	linear.setColorAt(1, QColor::fromRgb(0, 102, 0));

	painter.setBrush(linear);
	painter.setPen(Qt::transparent);
	painter.drawRect(0, 0, canvas->width(), canvas->height());

	terrProg = canvas->toImage();
}
