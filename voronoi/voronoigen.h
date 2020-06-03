#ifndef VORONOIGEN_H
#define VORONOIGEN_H

#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <climits>

#include <QDebug>

#include "voronoimap.h"
#include "sweepline.h"
#include "FastNoise.h"

class VoronoiGen
{
public:
	VoronoiGen();
	VoronoiGen(voronoiMap::Voronoi *vmap);
	~VoronoiGen();

	voronoiMap::Voronoi *vmap = nullptr;
	SweepLine *sweepLine = nullptr;
	FastNoise *mamemakiNoise;
	FastNoise *perlinNoise;
	std::vector<std::vector<voronoiMap::Point>> pointMap;

	void setVmap(voronoiMap::Voronoi *vmap);
	void clearVmap();
	/*!
	 * \brief mamemaki
	 * \param range
	 * \param threshold [-1.0~1.0]
	 */
	void mamemaki(const voronoiMap::Rectangle&& range, double threshold);
	int getSeed() const;
	void stepFortune();
	void performFortune();
	void performLloyd();
	void generateTerrain();
	void generateWaters(double range = 1.0);

	int getMaxAltitude();

private:
	int seed;
	int mamemakiOffset = 100;
	int maxAltitude = 256;
	int mapWidthX = 256;
	int mapWidthY = 256;
	std::vector<std::vector<double>> interpolateM = {
		{0.9,	0.95,	0.9},
		{0.95,	1.0,	0.95},
		{0.9,	0.95,	0.9}
	};
};

#endif // VORONOIGEN_H
