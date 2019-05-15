#include "sweepline.h"

using namespace voronoiMap;

Parabola::Parabola()
{

}

Parabola::~Parabola()
{

}

Event::Event()
{

}

Event::~Event()
{

}

SweepLine::SweepLine()
{
}

SweepLine::~SweepLine()
{

}

void SweepLine::addSite(Event *eve)
{

}

double SweepLine::nextEvent()
{

}

void SweepLine::beachAdd(Parabola *para)
{

}

void SweepLine::dealCircleEvent(Event *eve)
{

}

void SweepLine::finishEdges()
{

}

double SweepLine::parabolaX(const Point& focus, double y)
{
	double k = focus.y;
	double h = (L + focus.x) / 2.0;
	double c = -(L - focus.x) / 2.0;
	if (c >= 0)
	{
		return LMINVALUE;
	}
	else
	{
		double x = pow(y - k, 2) / (4 * c) + h;
		return x;
	}
}

PointF SweepLine::getIntersect(PointF *a, PointF *b)
{

}
