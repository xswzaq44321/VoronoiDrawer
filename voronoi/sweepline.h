#ifndef SWEEPLINE_H
#define SWEEPLINE_H

#include <list>
#include <queue>
#include "voronoimap.h"

constexpr double LMINVALUE = -1e100;
constexpr double LMAXVALUE = 1e100;
constexpr float MINVALUE = -1e30;
constexpr float MAXVALUE = 1e30;

class Parabola
{
public:
	Parabola();
	~Parabola();

	PointF focus;
	Event* event;

};

class Event
{
public:
	Event();
	~Event();

	std::list<Parabola*> relevants;
	double x;
	PointF center;
	bool isCircle;
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
	double parabolaX(const Point& focus, double y);
	PointF getIntersect(PointF* a, PointF* b);
};

#endif // SWEEPLINE_H
