#ifndef VORONOIMAP_H
#define VORONOIMAP_H

#include <vector>
#include <utility>	//std::pair
#include <exception>
#include <algorithm>
#include <set>
#include "json.hpp"
#define PI 3.1415926535

namespace voronoiMap {

class Rectangle;
struct Point;
struct PointF;
class Edge;
class Polygon;
class Voronoi;

class Rectangle
{
public:
	Rectangle();
	Rectangle(int x, int y, int width, int height);
	~Rectangle() = default;

	int x, y;
	int width, height;

	bool contains(int x, int y) const;
	int getRight() const;
	int getBottom() const;
};

struct Point
{
	Point();
	Point(int x, int y);
	Point(const Point* old);
	Point(const Point& old);
	Point(const PointF* old);
	Point(const PointF& old);
	~Point() = default;

	int x;
	int y;

	double distance(const Point& other) const;
	explicit operator PointF() const;
	bool operator()(const Point& lhs, const Point& rhs);
};

bool operator<(const Point& lhs, const Point& rhs);

struct PointF
{
	PointF();
	PointF(double x, double y);
	PointF(const PointF* old);
	PointF(const PointF& old);
	PointF(const Point* old);
	PointF(const Point& old);
	~PointF() = default;

	double x;
	double y;

	double distance(const PointF& other) const;
	explicit operator Point() const;
};

class Edge
{
public:
	Edge();
	Edge(int id1, int id2, bool is_abs = false);
	Edge(const Edge* old);
	Edge(const Edge& old);
	~Edge();

	Point *a; // start point
	Point *b; // end point
	int parentID[2] = {-1, -1};

	Point* getParentID(const int& index);
	bool hasParentID(const int id);
	double Distance(const Point& other) const;
	bool isAbstract() const;
	void deAbstract();

private:
	bool is_abstract;
};

class Polygon
{
public:
	Polygon();
	Polygon(const Polygon* old);
	Polygon(const Polygon& old);
	Polygon(Point* f);
	Polygon(double focusx, double focusy);
	~Polygon();

	std::vector<Edge*> edges;
	Point *focus;
	int id = -1;

	bool contains(const Point& other); // stump
	bool contains(const Point& other) const; // stump
	void organize();
	bool isComplete();
private:
	bool organized;
};

class Voronoi
{
public:
	Voronoi();
	Voronoi(int width, int height);
//	Voronoi(const std::string& json_map);
	Voronoi(const Voronoi* old);
	Voronoi(const Voronoi& old);
	~Voronoi();

	int width;
	int height;
	std::vector<Polygon*> polygons;

	std::string toJson(int indent);
	static Voronoi* fromJson(const std::string&& json_map);
	Polygon* addPoly(Polygon* p);
};

/**
 * @brief getVector calculates vector form a to b
 * @param a start point
 * @param b end point
 * @return a vector from a to b
 */
Point getVector(const Point& a, const Point& b);
/**
 * @brief cross calculates cross value of points a, b, o
 * @param a
 * @param b
 * @param o
 * @return cross value of vector oa x ob
 */
double cross(const Point& a, const Point& b, const Point& o);
PointF midPoint(const std::vector<Point>& points);

template<typename T1, typename T2>
void pairsort(T1 a[], T2 b[], size_t n);

template<typename T1, typename T2, class Compare>
void pairsort(T1 a[], T2 b[], size_t n, Compare comp);

}

#endif // VORONOIMAP_H
