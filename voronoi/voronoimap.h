#ifndef VORONOIMAP_H
#define VORONOIMAP_H

#include <vector>
#include <utility> //std::pair
#include "json.hpp"

namespace voronoiMap {

class Rectangle{
public:
	Rectangle();
	Rectangle(int x, int y, int width, int height);
	~Rectangle();

	int x, y;
	int width, height;
	int right, bottom;

	bool contains(int x, int y);
};

struct Point
{
public:
	Point();
	Point(int x, int y);
	~Point();

	int x;
	int y;

	double distance(const Point& other);
};

struct PointF
{
	PointF();
	~PointF();

	double x;
	double y;

	double distance(const PointF& other);
	operator Point() const;
};

class Edge
{
public:
	Edge();
	~Edge();

	Point *a;
	Point *b;
	int parentID[2] = {-1, -1};

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
	Point *focus;
	int id = -1;

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
	Voronoi(const std::string& json_map);
	~Voronoi();

	int width;
	int height;
	std::vector<Polygon*> polygons;

	std::string toJson(int indent);
};

Point getVector(const Point& a, const Point& b);
double cross(const Point& a, const Point& b, const Point& o);
Point midPoint(const Point[]);

template<typename T1, typename T2>
void pairsort(T1 a[], T2 b[], int n);

}

#endif // VORONOIMAP_H
