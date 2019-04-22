#ifndef VORONOIMAP_H
#define VORONOIMAP_H

#include <vector>

namespace voronoiMap {

struct Point
{
public:
	Point();
	Point(double x, double y);
	~Point();

	int x;
	int y;

	double distance(const Point& other);
};

class Edge
{
public:
	Edge();
	~Edge();

	Point *a;
	Point *b;
	int parentId[2];

	Point& get(const int& index);
	double Distance(const Point& other);
};

class Polygon
{
public:
	Polygon();
	Polygon(double focusx, double focusy);
	~Polygon();

	std::vector<Edge*> edges;
	Point *Focus;
	int id;

	bool contains(const Point& other);
private:
	bool organized;

	void organize();
};

class Voronoi
{
public:
	Voronoi();
	Voronoi(int width, int height);
	~Voronoi();

	int width;
	int height;
	std::vector<Polygon*> polygons;

	void addPoly(double focusx, double focusy);
	void addPoly(const Polygon& other);
};

Point getVector(const Point& a, const Point& b);
double cross(const Point& a, const Point& b, const Point& o);
Point midPoint(const Point[]);

}

#endif // VORONOIMAP_H
