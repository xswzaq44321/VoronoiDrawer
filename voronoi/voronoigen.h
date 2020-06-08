#ifndef VORONOIGEN_H
#define VORONOIGEN_H

#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <functional>
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
	FastNoise *riverNoise;
	std::vector<std::vector<voronoiMap::Point>> pointMap;

	/*!
	 * \brief loadScript
	 * \param path
	 * \throws std::runtime_error
	 */
	void loadScript(std::string path = "mods");
	void setVmap(voronoiMap::Voronoi *vmap);
	void clearVmap();
	void mamemaki(const voronoiMap::Rectangle&& range);
	int getSeed() const;
	void stepFortune();
	void performFortune();
	void performLloyd();
	void generateTerrain();
	void generateWaters(double range = 1.0);
	void generateBiomes();

	int getMaxAltitude();

	friend class Smooth;
	class Smooth{
	public:
		Smooth(VoronoiGen *parent);
		void loadScript(std::string path);
		void interpolateMethod(int iteration);
		std::vector<std::vector<double>> interpolateM;
	private:
		VoronoiGen *parent;
		std::function<sol::table(sol::table)> scriptMethod = {};
	}smooth;
	friend class Biomes;
	class Biomes{
	public:
		Biomes(VoronoiGen *parent);
		void loadScript(std::string path);
		std::function<sol::table(sol::table)> setBiome;
	private:
		VoronoiGen *parent;
	}biomes;

private:
	sol::state lua;
	int seed;
	int maxAltitude;
	int mapWidthX;
	int mapWidthY;
	std::function<sol::table(sol::table)> terrainScriptMethod;

	int currentChunk[2];
	float getNoise(float x, float y);
};

#endif // VORONOIGEN_H
