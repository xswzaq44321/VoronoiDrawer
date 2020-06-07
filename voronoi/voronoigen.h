#ifndef VORONOIGEN_H
#define VORONOIGEN_H

#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <climits>

#define QT
#ifdef QT
#include <QDebug>
#endif

#include "voronoimap.h"
#include "sweepline.h"
#include "FastNoise.h"
#include "dependencies/sol.hpp"

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

	/*!
	 * \brief loadScript
	 * \param path
	 * \throws std::runtime_error
	 */
	void loadScript(std::string path);
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

	class Smooth{
	public:
		Smooth(VoronoiGen *parent);
		void loadScript();
		void interpolateMethod();
		std::vector<std::vector<double>> interpolateM;
	private:
		VoronoiGen *parent;
	}smooth;
	friend class Smooth;

private:
	sol::state lua;
	int seed;
	int mamemakiOffset;
	int maxAltitude;
	int mapWidthX;
	int mapWidthY;
};

#endif // VORONOIGEN_H
