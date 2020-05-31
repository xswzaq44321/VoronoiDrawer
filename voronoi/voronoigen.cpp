#include "voronoigen.h"

using namespace voronoiMap;
using namespace std;

VoronoiGen::VoronoiGen():
	VoronoiGen(nullptr)
{
}

VoronoiGen::VoronoiGen(Voronoi *vmap):
	vmap(vmap)
{
	random_device rd;
	default_random_engine gen = std::default_random_engine(rd());
	uniform_int_distribution<int> dis(INT_MIN, INT_MAX);
	this->seed = dis(gen);
	mamemakiNoise = new FastNoise(seed);
	perlinNoise = new FastNoise(seed);
	qDebug() << "seed = " << seed;

	if(vmap){
		pointMap.resize(vmap->width, std::vector<voronoiMap::Point>(vmap->height));
		mapWidthX = vmap->width;
		mapWidthY = vmap->height;
	}
}

VoronoiGen::~VoronoiGen()
{
	delete vmap;
	delete sweepLine;
	delete mamemakiNoise;
	delete perlinNoise;
}

void VoronoiGen::setVmap(voronoiMap::Voronoi *vmap)
{
	delete this->vmap;
	delete this->sweepLine;
	this->vmap = vmap;
	this->sweepLine = nullptr;
	pointMap.resize(vmap->width, std::vector<voronoiMap::Point>(vmap->height));
	mapWidthX = vmap->width;
	mapWidthY = vmap->height;
}

void VoronoiGen::clearVmap()
{
	delete this->vmap;
	delete this->sweepLine;
	this->vmap = nullptr;
	this->sweepLine = nullptr;
}

void VoronoiGen::stepFortune()
{
	if(vmap == nullptr)
		return;
	if(sweepLine == nullptr)
		sweepLine = new SweepLine(vmap);
	if(sweepLine->nextEvent() == sweepLine->LMAXVALUE){
		sweepLine->finishEdges();
		for(Polygon* poly : vmap->polygons){
			poly->organize();
		}
	}
}

void VoronoiGen::performFortune()
{
	if(vmap == nullptr)
		return;
	if(sweepLine == nullptr){
		for (auto && it: vmap->polygons) {
			it->edges.clear();
		}
		sweepLine = new SweepLine(vmap);
	}
	while(sweepLine->nextEvent() != sweepLine->LMAXVALUE);
	sweepLine->finishEdges();
//	for(Polygon* poly : vmap->polygons){
//		poly->organize();
//	}
	delete sweepLine;
	sweepLine = nullptr;
}

void VoronoiGen::performLloyd()
{
	voronoiMap::Voronoi *newMap = new Voronoi(vmap->width, vmap->height);
	for(const auto& it : vmap->polygons){
		if(it->isComplete()){
			vector<Point*> points;
			for(const auto& iit : it->edges){
				points.push_back(iit->a);
				points.push_back(iit->b);
			}
			int cx = 0;
			int cy = 0;
			for(const auto& iit : points){
				cx += iit->x;
				cy += iit->y;
			}
			cx = cx / (double)points.size();
			cy = cy / (double)points.size();
			newMap->addPoly(new Polygon(new Point(cx, cy)));
		}else{
			newMap->addPoly(new Polygon(new Point(it->focus)));
		}
	}
	setVmap(newMap);
	delete sweepLine;
	sweepLine = nullptr;
}

void VoronoiGen::generateTerrain()
{
	for (auto&& it : vmap->polygons) {
		for (auto&& jt : it->edges) {
			if(jt->isAbstract() || jt->b == nullptr)
				continue;
			Point *a = jt->a;
			a->terrain.altitude = (perlinNoise->GetNoise(a->x, a->y) + 1) / 2.0 * maxAltitude;
			Point *b = jt->b;
			b->terrain.altitude = (perlinNoise->GetNoise(b->x, b->y) + 1) / 2.0 * maxAltitude;
		}
	}

	// interpolation terrains
	for (auto&& poly : vmap->polygons) {
		if(!poly->isComplete())
			continue;
		int minX, maxX, minY, maxY;
		minX = mapWidthX;
		minY = mapWidthY;
		maxX = maxY = 0;
		for (const auto& edge : poly->edges) {
			for (Point* Edge::*it : {&Edge::a, &Edge::b}) {
				if(minX > (edge->*it)->x)	minX = (edge->*it)->x;
				if(minY > (edge->*it)->y)	minY = (edge->*it)->y;
				if(maxX < (edge->*it)->x)	maxX = (edge->*it)->x;
				if(maxY < (edge->*it)->y)	maxY = (edge->*it)->y;
			}
		}
		maxX = std::min(maxX, mapWidthX);
		maxY = std::min(maxY, mapWidthY);
		minX = std::max(minX, 0);
		minY = std::max(minY, 0);
		for (int x = minX; x < maxX; ++x) {
			for (int y = minY; y < maxY; ++y) {
				if(poly->contains(x, y)){
					double sumBase = 0, sum = 0;
					for (const auto& edge : poly->edges) {
						sumBase += 1.0 / edge->a->distance(Point(x, y));
						sumBase += 1.0 / edge->b->distance(Point(x, y));
					}
//					sumBase /= 2.0;
					for (const auto& edge : poly->edges) {
						sum += (1.0 / edge->a->distance(Point(x, y))) * edge->a->terrain.altitude;
						sum += (1.0 / edge->b->distance(Point(x, y))) * edge->b->terrain.altitude;
					}
//					sum /= 2.0;
					pointMap[x][y].terrain.altitude = sum / sumBase;
				}
			}
		}
	}
}

void VoronoiGen::mamemaki(const Rectangle&& range, double threshold)
{
	vector<pair<float, Point>> v;
	for (int i = -mamemakiOffset; i < range.width + mamemakiOffset; ++i) {
		for (int j =  -mamemakiOffset; j < range.height + mamemakiOffset; ++j) {
			int x, y;
			x = i + range.x;
			y = j + range.y;
			if(mamemakiNoise->GetWhiteNoise(x, y) >= threshold){
				vmap->addPoly(new Polygon(new Point(x, y)));
			}
		}
	}
}

int VoronoiGen::getSeed() const
{
	return seed;
}
