#ifndef SWEEPLINE_H
#define SWEEPLINE_H

#include <list>
#include <queue>
#include "voronoimap.h"

constexpr double LMINVALUE = -1e100;
constexpr double LMAXVALUE = 1e100;
constexpr float MINVALUE = -1e30;
constexpr float MAXVALUE = 1e30;

class Event;

class Parabola
{
public:
	Parabola();
	~Parabola();

	voronoiMap::PointF* focus;
	Event* event;
};

class Event
{
public:
	Event();
	~Event();

	std::list<Parabola*> relevants;
	double x = -1;
	voronoiMap::PointF center();
	bool isCircle = false;
};

class SweepLine
{
public:
	SweepLine();
	~SweepLine();

	double L;
	std::list<Parabola*> beachParas;
	std::priority_queue<Event*> siteEvents;
	std::priority_queue<Event*> circleEvents;
	voronoiMap::Voronoi* vmap;

	void addSite(Event* eve);
	double nextEvent();
	void beachAdd(Parabola* para);
	void dealCircleEvent(Event* eve);
	void finishEdges();
	/**
	 * @brief parabolaX get current parabora focus's x coordinate on y
	 * @param focus
	 * @param y
	 * @return
	 */
	double parabolaX(const voronoiMap::Point& focus, double y);
	voronoiMap::PointF getIntersect(voronoiMap::PointF* a, voronoiMap::PointF* b);
};

#endif // SWEEPLINE_H
