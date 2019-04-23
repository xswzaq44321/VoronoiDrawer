#ifndef VORONOIMAP_H
#define VORONOIMAP_H

#include <vector>
#include <utility> //std::pair
#include "json.hpp"

namespace voronoiMap {

class Rectangle{
	public:
		Rectangle(int x, int y, int width, int height){
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
			this->Right = x + width;
			this->Bottom = y + height;
		}

		int x,y;
		int width,height;
		int Right, Bottom;

		bool Contains(int x, int y){
			if(x > this->x && x < this->Right){
				if(y > this->y && y < this->Bottom){
					return true;
				}
			}
			return false;
		}
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

	void addPoly(double focusx, double focusy);
	void addPoly(const Polygon& other);
	std::string toJson(int indent);
};

Point getVector(const Point& a, const Point& b);
double cross(const Point& a, const Point& b, const Point& o);
Point midPoint(const Point[]);

template<typename T1, typename T2>
void pairsort(T1 a[], T2 b[], int n);

}

#endif // VORONOIMAP_H
