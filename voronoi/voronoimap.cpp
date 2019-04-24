#include "voronoimap.h"

using namespace voronoiMap;
using json = nlohmann::json;

Rectangle::Rectangle()
{
}

Rectangle::Rectangle(int x, int y, int width, int height):
	x(x),y(y),
	width(width),height(height)
{
	this->right = x + width;
	this->bottom = y + height;
}

Rectangle::~Rectangle()
{
}

bool Rectangle::contains(int x, int y)
{
	if(x > this->x && x < this->right){
		if(y > this->y && y < this->bottom){
			return true;
		}
	}
	return false;
}


Point::Point()
{
}

Point::Point(int x, int y):
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

PointF::PointF()
{

}

PointF::~PointF()
{

}

double PointF::distance(const PointF &other)
{
	return 0;
}

PointF::operator Point() const
{
	return Point(x, y);
}


Edge::Edge():
	a(nullptr),
	b(nullptr)
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
	focus(nullptr)
{
}

Polygon::Polygon(double focusx, double focusy):
	focus(new Point(focusx, focusy))
{
}

Polygon::~Polygon()
{
	delete focus;
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

Voronoi::Voronoi(const std::string& json_map)
{
	json jj = json::parse(json_map);
	this->width = jj["width"];
	this->height = jj["height"];
	for(auto it:jj["polygons"]){
		Polygon poly;
		poly.id = it["id"].get<int>();
		poly.focus = new Point(it["focus"]["x"].get<int>(), it["focus"]["y"].get<int>());
		for(auto jt:it["edges"]){
			Edge edg;
			int ax = jt["a"]["x"].get<int>();
			int ay = jt["a"]["y"].get<int>();
			int bx = jt["b"]["x"].get<int>();
			int by = jt["b"]["y"].get<int>();
			edg.a = new Point(ax, ay);
			edg.b = new Point(bx, by);
			edg.parentID[0] = jt["parentID"][0].get<int>();
			edg.parentID[1] = jt["parentID"][1].get<int>();
			poly.edges.push_back(new Edge(edg));
		}
		this->polygons.push_back(new Polygon(poly));
	}
}

Voronoi::~Voronoi()
{
	for(auto poly : polygons){
		delete poly;
	}
}

std::string Voronoi::toJson(int indent)
{
	json jj;
	for(auto it:polygons){
		json obj;
		obj["id"] = it->id;
		obj["focus"]["y"] = it->focus->y;
		obj["focus"]["x"] = it->focus->x;
		obj["edges"] = json::array();
		for(Edge* jt:it->edges){
			json edge;
			edge["line"]["b"]["y"] = jt->b->y;
			edge["line"]["b"]["x"] = jt->b->x;
			edge["line"]["a"]["y"] = jt->a->y;
			edge["line"]["a"]["x"] = jt->a->x;
			edge["parentID"][0] = jt->parentID[0];
			edge["parentID"][1] = jt->parentID[1];
			obj["edges"].push_back(edge);
		}
		jj["polygons"].push_back(obj);
	}
	jj["height"] = height;
	jj["width"] = width;
	if(indent == 0){
		return jj.dump();
	}else{
		return jj.dump(indent);
	}
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

template<typename T1, typename T2>
void pairsort(T1 a[], T2 b[], int n)
{
	std::pair<T1, T2> pairArr[n];

	// Storing the respective array
	// elements in pairs.
	for (int i = 0; i < n; i++)
	{
		pairArr[i].first = a[i];
		pairArr[i].second = b[i];
	}

	// Sorting the pair array.
	std::sort(pairArr, pairArr + n);

	// Modifying original arrays
	for (int i = 0; i < n; i++)
	{
		a[i] = pairArr[i].first;
		b[i] = pairArr[i].second;
	}
}
