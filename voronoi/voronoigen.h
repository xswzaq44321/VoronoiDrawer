#ifndef VORONOIGEN_H
#define VORONOIGEN_H

#include <vector>
#include <numeric>
#include <algorithm>

#include "voronoimap.h"
#include "sweepline.h"
#include "FastNoise.h"

class VoronoiGen
{
public:
	VoronoiGen() = default;
	VoronoiGen(voronoiMap::Voronoi *vmap);
	~VoronoiGen();

	voronoiMap::Voronoi *vmap = nullptr;
	SweepLine *sweepLine = nullptr;
	FastNoise myNoise;

	void setVmap(voronoiMap::Voronoi *vmap);
	void clearVmap();
	void stepFortune();
	void performFortune();
	void performLloyd();
	void mamemaki(voronoiMap::Rectangle&& range, int count);
};

#endif // VORONOIGEN_H
