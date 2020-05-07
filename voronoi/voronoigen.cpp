#include "voronoigen.h"

using namespace voronoiMap;
using namespace std;

VoronoiGen::VoronoiGen(Voronoi *vmap):
	vmap(vmap)
{
}

VoronoiGen::~VoronoiGen()
{
	delete vmap;
	delete sweepLine;
}

void VoronoiGen::setVmap(voronoiMap::Voronoi *vmap)
{
	delete this->vmap;
	delete this->sweepLine;
	this->vmap = vmap;
	this->sweepLine = nullptr;
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
	if(sweepLine == nullptr)
		sweepLine = new SweepLine(vmap);
	while(sweepLine->nextEvent() != sweepLine->LMAXVALUE);
	sweepLine->finishEdges();
//	for(Polygon* poly : vmap->polygons){
//		poly->organize();
//	}
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

void VoronoiGen::mamemaki(Rectangle&& range, int count)
{
	vector<pair<float, Point>> v;
	for (int i = 0; i < range.width; ++i) {
		for (int j = 0; j < range.height; ++j) {
			int x, y;
			x = i + range.x;
			y = j + range.y;
			v.push_back(make_pair(myNoise.GetWhiteNoise(x, y), Point(x, y)));
		}
	}
	std::sort(v.begin(), v.end(), [](pair<float, Point> lhs, pair<float, Point> rhs){
		return lhs.first < rhs.first;
	});
	for (int i = 0; i < count; ++i) {
		auto temp = v[i].second;
		vmap->addPoly(new Polygon(new Point(v[i].second)));
	}
}
