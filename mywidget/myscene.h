#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QDebug>
#include <QSize>
#include <QGraphicsSceneMouseEvent>
#include <QVector>
#include <QFileDialog>
#include <memory>

#include "voronoi/voronoimap.h"
#include "mygraphicsellipseitem.h"
#include "voronoi/sweepline.h"

class MyScene : public QGraphicsScene
{
public:
	MyScene();
	MyScene(const QSize &size);
	~MyScene();

	voronoiMap::Voronoi *vmap = nullptr;
	SweepLine *sweepLine = nullptr;
	QVector<std::shared_ptr<MyGraphicsEllipseItem>> ellipseItems;
	QVector<std::shared_ptr<QGraphicsLineItem>> lineItems;

//	voronoiMap::Voronoi* createVmap(); // to be remove
	/*!
		\brief creates a blank vmap
	*/
	void setVmap();
	/*!
		\brief set vmap and sync content
		\param vmap
	*/
	void setVmap(voronoiMap::Voronoi* vmap);
	/*!
		\brief clearContent clear scene and vmap
	*/
	void clearContent();
	/*!
		\brief syncVmap sync scene objects to vmap
	*/
	void syncVmap();
	/*!
	 * \brief syncFortune
	 */
	void syncFortune();

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
