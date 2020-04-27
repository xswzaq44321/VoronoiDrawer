#ifndef SWEEPLINE_H
#define SWEEPLINE_H

#include <list>
#include <queue>
#include <vector>
#include <functional>
#include <algorithm>
#include "voronoimap.h"

#include <QDebug>

class Event;

class Parabola
{
public:
	Parabola(voronoiMap::Point* focus, voronoiMap::Polygon* poly);
	~Parabola();

	voronoiMap::PointF* focus;
	voronoiMap::Polygon* poly;
	Event* event;
};

class Event
{
public:
	Event(voronoiMap::Polygon* rel);
	Event(Parabola* r1, Parabola* r2, Parabola* r3);
	~Event();

	std::vector<Parabola*> relevants;
	double x = -1;
	voronoiMap::PointF center;
	bool isCircle = false;
	bool valid = true;
	friend bool operator<(const Event& lhs, const Event& rhs);
	friend bool operator>(const Event& lhs, const Event& rhs);
};

class EventCmp
{
public:
	bool operator()(Event* lhs, Event* rhs)
	{
		return lhs->x > rhs->x;
	}
};

template<typename T, class allo, class cmp>
class my_priority_queue: public std::priority_queue<T, allo, cmp>{
public:
	bool remove(const T& value) {
		auto it = std::find(this->c.begin(), this->c.end(), value);
		if (it != this->c.end()) {
			this->c.erase(it);
			std::make_heap(this->c.begin(), this->c.end(), this->comp);
			return true;
		}
		else {
			return false;
		}
	}
	void removeAll(std::function<bool(T)> pred){
		c.erase(std::remove_if(c.begin(), c.end(), pred), c.end());
		std::make_heap(c.begin(), c.end(), comp);
	}
};

class SweepLine
{
public:
	SweepLine();
	SweepLine(voronoiMap::Voronoi* vmap);
	~SweepLine();

	double L;
	std::vector<Parabola*> beachParas;
	std::priority_queue<Event*, std::vector<Event*>, EventCmp> siteEvents;
	my_priority_queue<Event*, std::vector<Event*>, EventCmp> circleEvents;
	voronoiMap::Voronoi* vmap;

	void addSite(Event* eve);
	double nextEvent();
	void beachAdd(Parabola* para);
	void dealCircleEvent(Event* eve);
	void finishEdges();
	/*!
	 \fn parabolaX
	 \brief parabolaX get current parabora focus's x coordinate on y
	 \param focus
	 \param y
	 \return
	*/
	double parabolaX(const voronoiMap::Point& focus, double y);
	voronoiMap::PointF getIntersect(voronoiMap::PointF* a, voronoiMap::PointF* b);
	voronoiMap::PointF getIntersect(voronoiMap::Point *a, voronoiMap::Point *b);


public:
	const double LMINVALUE = -1e100;
	const double LMAXVALUE = 1e100;
	const float MINVALUE = -1e30;
	const float MAXVALUE = 1e30;
};

class calculator{
public:
	static double det(double m[3][3]);
	static double distance(voronoiMap::PointF a, voronoiMap::PointF b);
};

#endif // SWEEPLINE_H
