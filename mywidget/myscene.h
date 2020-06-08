#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <QDebug>
#include <QSize>
#include <QGraphicsSceneMouseEvent>
#include <QVector>
#include <QFileDialog>
#include <QPainter>
#include <memory>
#include <cmath>
#include <algorithm>

#include "voronoi/voronoimap.h"
#include "mygraphicsellipseitem.h"
#include "voronoi/sweepline.h"
#include "voronoi/voronoigen.h"

class MyScene : public QGraphicsScene
{
public:
	MyScene(const QSize &size);
	MyScene(voronoiMap::Voronoi* vmap);
	~MyScene();

	VoronoiGen voronoiGen;
	QVector<std::shared_ptr<MyGraphicsEllipseItem>> ellipseItems;
	QVector<std::shared_ptr<QGraphicsLineItem>> lineItems;
	QVector<std::shared_ptr<QGraphicsItem>> assistantItems;
	QPixmap* mapCanvas = nullptr;

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
	void syncVmap(bool updatePoint = true);
	/*!
	 * \brief syncFortune
	 */
	void syncFortune();
	void drawTerrain(float altitudeMax);
	void drawWater();
	void drawBiome();
	void setAutoFortune(bool action);

	// QGraphicsScene interface
protected:
	void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
	void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
	void dropEvent(QGraphicsSceneDragDropEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:
	void initialProgression();

	bool autoFortune = false;
	QImage terrProg; // terrain color progress
	QGraphicsPixmapItem *mapCanvasItem = nullptr;
};

#endif // MYSCENE_H
