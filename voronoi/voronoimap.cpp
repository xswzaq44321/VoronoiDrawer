#include "voronoimap.h"

using namespace voronoiMap;
using json = nlohmann::json;

Rectangle::Rectangle():
	x(0),y(0),width(0),height(0)
{
}

Rectangle::Rectangle(int x, int y, int width, int height):
	x(x),y(y),
	width(width),height(height)
{
}

bool Rectangle::contains(int x, int y) const
{
	if(x > this->x && x < this->getRight()){
		if(y > this->y && y < this->getBottom()){
			return true;
		}
	}
	return false;
}

int Rectangle::getRight() const
{
	return x + width;
}

int Rectangle::getBottom() const
{
	return y + height;
}

Point::Point()
{
}

Point::Point(int x, int y):
	x(x),
	y(y)
{
}

Point::Point(const Point *old):
	x(old->x), y(old->y)
{
}

Point::Point(const Point &old):
	x(old.x), y(old.y)
{
}

Point::Point(const PointF *old):
	x(old->x), y(old->y)
{
}

Point::Point(const PointF &old):
	x(old.x), y(old.y)
{
}

double Point::distance(const Point &other) const
{
	return sqrt(pow(this->x - other.x, 2) + pow(this->y - other.y, 2));
}

#if QT_VERSION >= 0X040000
Point::operator QPoint() const
{
	return QPoint(x, y);
}

voronoiMap::Point::operator QPointF() const
{
	return QPointF(x, y);
}
#endif

PointF::PointF()
{
}

PointF::PointF(double x, double y):
	x(x),y(y)
{
}

PointF::PointF(const PointF *old):
	x(old->x), y(old->y)
{
}

PointF::PointF(const PointF &old):
	x(old.x), y(old.y)
{
}

PointF::PointF(const Point *old):
	x(old->x), y(old->y)
{
}

PointF::PointF(const Point &old):
	x(old.x), y(old.y)
{
}

double PointF::distance(const PointF &other) const
{
	return sqrt(pow(this->x - other.x, 2) + pow(this->y - other.y, 2));
}

PointF::operator Point() const
{
	return Point(x, y);
}

#if QT_VERSION >= 0x040000
PointF::operator QPoint() const
{
	return QPoint(x, y);
}

PointF::operator QPointF() const
{
	return QPointF(x, y);
}
#endif

Edge::Edge():
	a(nullptr),
	b(nullptr)
{
}

Edge::Edge(int id1, int id2, bool is_abs):
	Edge()
{
	this->parentID[0] = id1;
	this->parentID[1] = id2;
	this->is_abstract = is_abs;
}

Edge::Edge(const Edge *old)
{
	this->a = old->a ? new Point(old->a) : nullptr;
	this->b = old->b ? new Point(old->b) : nullptr;
	this->parentID[0] = old->parentID[0];
	this->parentID[1] = old->parentID[1];
	this->is_abstract = old->is_abstract;
}

Edge::Edge(const Edge &old)
{
	this->a = new Point(old.a);
	this->b = new Point(old.b);
	this->parentID[0] = old.parentID[0];
	this->parentID[1] = old.parentID[1];
	this->is_abstract = old.is_abstract;
}

Edge::~Edge()
{
	delete a;
	delete b;
}

Point* Edge::getParentID(const int &index)
{
	if(index == 0)
		return a;
	else if(index == 1)
		return b;
	else
		throw std::out_of_range("Edge::get: index must be in the range of [0, 1]");
}

bool Edge::hasParentID(const int id)
{
	return std::find(parentID, parentID + 2, id) != (parentID + 2);
}

double Edge::Distance(const Point &other) const
{
	if(b == nullptr || a == nullptr)
		throw std::invalid_argument("Edge::Distance: either a or b is null");
	double bar = abs((b->y - a->y)*other.x - (b->x - a->x)*other.y + b->x*a->y - b->y*a->x);
	double bar2 = sqrt(pow(b->y - a->y, 2) + pow(b->x - a->x, 2));
	return bar / bar2;
}

bool Edge::isAbstract()
{
	return is_abstract;
}

void Edge::deAbstract()
{
	this->is_abstract = false;
}

Polygon::Polygon():
	focus(nullptr),
	organized(false)
{
}

Polygon::Polygon(const Polygon *old)
{
	for(auto edge : old->edges){
		this->edges.push_back(new Edge(edge));
	}
	this->focus = new Point(old->focus);
	this->id = old->id;
	// if old one is organized, since we are copying edges in order,
	// new one must also been organized, vice versa.
	this->organized = old->organized;
}

Polygon::Polygon(const Polygon &old)
{
	for(auto edge : old.edges){
		this->edges.push_back(new Edge(edge));
	}
	this->focus = new Point(old.focus);
	this->id = old.id;
	// if old one is organized, since we are copying edges in order,
	// new one must also been organized, vice versa.
	this->organized = old.organized;
}

Polygon::Polygon(double focusx, double focusy):
	focus(new Point(focusx, focusy))
{
}

Polygon::~Polygon()
{
	delete focus;
	for(Edge* edge : edges){
		delete edge;
	}
}

bool Polygon::contains(const Point &other)
{
	if(!organized)
		organize();
	return false;
}

bool Polygon::contains(const Point &other) const
{
	return false;
}

void Polygon::organize()
{
	double* edgeDegrees = new double[edges.size()];
	Edge** edgeArr = &edges[0]; // vector to array
	for (int i = 0; i < edges.size(); i++)
	{
		// add average degree of edge to array for sorting uses later
		double ta = atan2(edges[i]->a->y - focus->y, edges[i]->a->x - focus->x);
		double tb = atan2(edges[i]->b->y - focus->y, edges[i]->b->x - focus->x);
		double degree = (ta + tb) / 2;
		if (abs(ta - tb) > PI)
		{
			degree += PI;
			for (; degree > PI; degree -= 2 * PI) ;
			for (; degree < -PI; degree += 2 * PI) ;
		}
		edgeDegrees[i] = degree;

		// sort a, b
		if (degree < -PI / 2 || degree > PI / 2)
		{ // angle of edge is behind focus
			if (ta < 0 && tb > 0)
				ta += 2 * PI;
			else if (ta > 0 && tb < 0)
				tb += 2 * PI;
		}
		if (ta >= tb)
		{
			auto temp = edges[i]->a;
			edgeArr[i]->a = edges[i]->b;
			edgeArr[i]->b = temp;
		}
	}
	pairsort(edgeDegrees, edgeArr, edges.size());
	organized = true;
}

Voronoi::Voronoi()
{
}

Voronoi::Voronoi(int width, int height):
	width(width),
	height(height)
{
}

//Voronoi::Voronoi(const std::string& json_map)
//{
//	json jresult = json::parse(json_map);
//	this->width = jresult["width"];
//	this->height = jresult["height"];
//	for(auto it:jresult["polygons"]){
//		Polygon poly;
//		poly.id = it["id"].get<int>();
//		poly.focus = new Point(it["focus"]["x"].get<int>(), it["focus"]["y"].get<int>());
//		for(auto jt:it["edges"]){
//			Edge edg;
//			int ax = jt["a"]["x"].get<int>();
//			int ay = jt["a"]["y"].get<int>();
//			int bx = jt["b"]["x"].get<int>();
//			int by = jt["b"]["y"].get<int>();
//			edg.a = new Point(ax, ay);
//			edg.b = new Point(bx, by);
//			edg.parentID[0] = jt["parentID"][0].get<int>();
//			edg.parentID[1] = jt["parentID"][1].get<int>();
//			poly.edges.push_back(new Edge(edg));
//		}
//		this->polygons.push_back(new Polygon(poly));
//	}
//}

Voronoi::Voronoi(const Voronoi *old):
	width(old->width), height(old->height)
{
	for(auto poly : old->polygons){
		polygons.push_back(new Polygon(poly));
	}
}

Voronoi::Voronoi(const Voronoi &old)
{

}

Voronoi::~Voronoi()
{
	for(auto poly : polygons){
		delete poly;
	}
}

std::string Voronoi::toJson(int indent)
{
	json jresult;
	for(auto it:polygons){
		json obj;
		obj["id"] = it->id;
		obj["focus"]["y"] = it->focus->y;
		obj["focus"]["x"] = it->focus->x;
		obj["edges"] = json::array();
		for(Edge* jt:it->edges){
			json edge;
			edge["b"]["y"] = jt->b->y;
			edge["b"]["x"] = jt->b->x;
			edge["a"]["y"] = jt->a->y;
			edge["a"]["x"] = jt->a->x;
			edge["parentID"][0] = jt->parentID[0];
			edge["parentID"][1] = jt->parentID[1];
			obj["edges"].push_back(edge);
		}
		jresult["polygons"].push_back(obj);
	}
	jresult["height"] = height;
	jresult["width"] = width;
	if(indent == 0){
		return jresult.dump();
	}else{
		return jresult.dump(indent);
	}
}

Voronoi* Voronoi::fromJson(const std::string &&json_map)
{
	Voronoi *newMap = new Voronoi();
	json jresult = json::parse(json_map);
	newMap->width = jresult["width"];
	newMap->height = jresult["height"];
	for(auto it:jresult["polygons"]){
		Polygon *poly = new Polygon();
		poly->id = it["id"].get<int>();
		poly->focus = new Point(it["focus"]["x"].get<int>(), it["focus"]["y"].get<int>());
		for(auto jt:it["edges"]){
			Edge *edg = new Edge();
			int ax = jt["a"]["x"].get<int>();
			int ay = jt["a"]["y"].get<int>();
			int bx = jt["b"]["x"].get<int>();
			int by = jt["b"]["y"].get<int>();
			edg->a = new Point(ax, ay);
			edg->b = new Point(bx, by);
			edg->parentID[0] = jt["parentID"][0].get<int>();
			edg->parentID[1] = jt["parentID"][1].get<int>();
			poly->edges.push_back(edg);
		}
		newMap->polygons.push_back(poly);
	}
	return newMap;
}

Polygon* Voronoi::addPoly(Polygon *p)
{
	p->id = polygons.size();
	polygons.push_back(p);
	return p;
}

Point voronoiMap::getVector(const Point &a, const Point &b)
{
	return Point(b.x - a.x, b.y - a.y);
}

double voronoiMap::cross(const Point &a, const Point &b, const Point &o)
{
	Point v1 = getVector(o, a);
	Point v2 = getVector(o, b);
	return v1.x * v2.y - v1.y * v2.x;
}

PointF voronoiMap::midPoint(const std::vector<Point>& points)
{
	double x = 0, y = 0;
	for(auto point : points){
		x += point.x;
		y += point.y;
	}
	return PointF(x / points.size(), y / points.size());
}

template<typename T1, typename T2>
void voronoiMap::pairsort(T1 a[], T2 b[], int n)
{
	std::vector<std::pair<T1, T2>> pairArr(n);

	// Storing the respective array
	// elements in pairs.
	for (int i = 0; i < n; i++)
	{
		pairArr[i].first = a[i];
		pairArr[i].second = b[i];
	}

	// Sorting the pair array.
	std::sort(pairArr.begin(), pairArr.end());

	// Modifying original arrays
	for (int i = 0; i < n; i++)
	{
		a[i] = pairArr[i].first;
		b[i] = pairArr[i].second;
	}
}

template<typename T1, typename T2, class Compare>
void voronoiMap::pairsort(T1 a[], T2 b[], int n, Compare comp)
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
	std::sort(pairArr, pairArr + n, comp);

	// Modifying original arrays
	for (int i = 0; i < n; i++)
	{
		a[i] = pairArr[i].first;
		b[i] = pairArr[i].second;
	}
}
