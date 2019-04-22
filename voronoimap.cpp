#include "voronoimap.h"

using namespace voronoiMap;

Point::Point()
{
}

Point::Point(double x, double y):
	x(x),
	y(y)
{
}

Point::~Point()
{
}

double Point::distance(const Point &other)
{
	return 0;
}

Edge::Edge():
	a(nullptr),
	b(nullptr),
	parentId{-1, -1}
{
}

Edge::~Edge()
{
	delete a;
	delete b;
}

Point& Edge::get(const int &index)
{
	Point bar;
	return bar;
}

double Edge::Distance(const Point &other)
{
	return 0;
}

Polygon::Polygon():
	Focus(nullptr)
{
}

Polygon::Polygon(double focusx, double focusy):
	Focus(new Point(focusx, focusy))
{
}

Polygon::~Polygon()
{
	delete Focus;
	for(auto edge : edges){
		delete edge;
	}
}

bool Polygon::contains(const Point &other)
{
	return false;
}

Voronoi::Voronoi()
{
}

Voronoi::Voronoi(int width, int height):
	width(width),
	height(height)
{
}

Voronoi::~Voronoi()
{
	for(auto poly : polygons){
		delete poly;
	}
}

void Voronoi::addPoly(double focusx, double focusy)
{
	Polygon *bar = new Polygon(focusx, focusy);
	polygons.push_back(bar);
}

void Voronoi::addPoly(const Polygon &other)
{
}

Point getVector(const Point &a, const Point &b)
{
	return Point();
}

double cross(const Point &a, const Point &b, const Point &o)
{
	return 0;
}

Point midPoint(const Point[])
{
	return Point();
}
