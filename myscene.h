#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QDebug>
#include <QSize>
#include <QGraphicsSceneMouseEvent>

#include "voronoimap.h"
#include "mygraphicsellipseitem.h"

class MyScene : public QGraphicsScene
{
public:
	MyScene();
	MyScene(const QSize& size);
	~MyScene();

	voronoiMap::Voronoi *vmap;

	// QGraphicsScene interface
protected:
	void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
	void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
	void dropEvent(QGraphicsSceneDragDropEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MYSCENE_H
