#include "voronoigen.h"

using namespace voronoiMap;
using namespace std;

VoronoiGen::VoronoiGen():
	VoronoiGen(nullptr)
{}

VoronoiGen::VoronoiGen(Voronoi *vmap):
	vmap(vmap),
	smooth(this),
	biomes(this)
{
	random_device rd;
	default_random_engine gen = std::default_random_engine(rd());
//	gen.seed(time(NULL));
	uniform_int_distribution<int> dis(INT_MIN, INT_MAX);
	this->seed = dis(gen);
	mamemakiNoise = new FastNoise(seed);
	mamemakiNoise->SetNoiseType(FastNoise::WhiteNoise);
	perlinNoise = new FastNoise(seed);
	perlinNoise->SetNoiseType(FastNoise::Perlin);
	riverNoise = new FastNoise(seed);
	riverNoise->SetNoiseType(FastNoise::WhiteNoise);
	qDebug() << "seed = " << seed;

	if(vmap){
		pointMap.resize(vmap->width, std::vector<voronoiMap::Point>(vmap->height));
		mapWidthX = vmap->width;
		mapWidthY = vmap->height;
	}

	currentChunk[0] = 0;
	currentChunk[1] = 0;
	lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);
	loadScript();
}

VoronoiGen::~VoronoiGen()
{
	delete vmap;
	delete sweepLine;
	delete mamemakiNoise;
	delete perlinNoise;
}

void VoronoiGen::loadScript(string path)
{
	// terrain parameters
	lua.script_file(path + "/terrain.lua");
	maxAltitude = (int)lua["MaxAltitude"];
	mapWidthX = (int)lua["MapWidthX"];
	mapWidthY = (int)lua["MapWidthY"];
	perlinNoise->SetFrequency((float)lua["Noise"]["Frequency"]);
	lua["Noise"]["getNoise"] = [this](float x, float y){
		return this->getNoise(x, y);
	};
	auto func = lua["Noise"]["Method"];
	if(func.valid())
		terrainScriptMethod = (sol::function)func;
	else
		terrainScriptMethod = {};
	smooth.loadScript(path);
	biomes.loadScript(path);
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
	for (int x = 0; x < pointMap.size(); ++x) {
		for (int y = 0; y < pointMap.at(0).size(); ++y) {
			pointMap[x][y].terrain.height = 0;
		}
	}
	for (auto&& poly : vmap->polygons) {
		for (auto&& jt : poly->edges) {
			if(jt->isAbstract() || jt->b == nullptr)
				continue;
			Point *a = jt->a;
			a->terrain.height = (this->getNoise(a->x, a->y) + 1) / 2.0 * maxAltitude;
			Point *b = jt->b;
			b->terrain.height = (this->getNoise(b->x, b->y) + 1) / 2.0 * maxAltitude;
		}
		double sum = 0, avg;
		int count = 0;
		for (const auto& edge : poly->edges) {
			for (Point* Edge::*it : {&Edge::a, &Edge::b}) {
				sum += (edge->*it)->terrain.height;
				++count;
			}
		}
		avg = sum / count;
		poly->focus->terrain.height = avg;
	}
	// generating
	if(terrainScriptMethod){
		sol::table t = lua.create_table();
		for (int i = 0; i < pointMap.size(); ++i) {
			t[i + 1] = lua.create_table();
			for (int j = 0; j < pointMap.at(i).size(); ++j) {
				t[i + 1][j + 1] = lua.create_table();
				t[i + 1][j + 1]["height"] = pointMap[i][j].terrain.height;
			}
		}
		t = terrainScriptMethod(t);
		for (int i = 0; i < pointMap.size(); ++i) {
			for (int j = 0; j < pointMap.at(i).size(); ++j) {
				pointMap[i][j].terrain.height = t[i + 1][j + 1]["height"];
			}
		}
	}else{
		// interpolation terrains
		for (auto&& poly : vmap->polygons) {
			if(!poly->isComplete())
				continue;
			double sum = 0, avg;
			int count = 0;
			for (const auto& edge : poly->edges) {
				for (Point* Edge::*it : {&Edge::a, &Edge::b}) {
					sum += (edge->*it)->terrain.height;
					++count;
				}
			}
			avg = sum / count;
			poly->focus->terrain.height = avg;

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
							if(edge->a->distance(Point(x, y)) == 0){
								sum = edge->a->terrain.height;
								sumBase = 1.0;
								break;
							}
							if(edge->b->distance(Point(x, y)) == 0){
								sum = edge->b->terrain.height;
								sumBase = 1.0;
								break;
							}
							sum += (1.0 / edge->a->distance(Point(x, y))) * edge->a->terrain.height;
							sum += (1.0 / edge->b->distance(Point(x, y))) * edge->b->terrain.height;
							sumBase += 1.0 / edge->a->distance(Point(x, y));
							sumBase += 1.0 / edge->b->distance(Point(x, y));
						}
						pointMap[x][y].terrain.height = sum / sumBase;
						/*pointMap[x][y].terrain.altitude = avg*/;
					}
				}
			}
		}
	}

	// smoothing
	int iteration = (int)lua["Smooth"]["Iteration"];
	smooth.interpolateMethod(iteration);
}

void VoronoiGen::generateWaters(double range)
{
	for (int x = 0; x < pointMap.size(); ++x) {
		for (int y = 0; y < pointMap.at(0).size(); ++y) {
			pointMap[x][y].terrain.type = "";
		}
	}
	// river generation
	float startAltitude = (float)lua["RiverStartAltitude"] * maxAltitude;
	float riverThreshold = (float)lua["RiverThreshold"];
	auto cmp = [](Point* a, Point* b){return a->terrain.height < b->terrain.height;};
	for (auto&& poly : vmap->polygons) {
		for (const auto& edge : poly->edges) {
			if(edge->isRiver)
				continue;
			if(edge->a->terrain.height < startAltitude
					&& edge->b->terrain.height < startAltitude)
				continue;
			Point* upper = std::max(edge->a, edge->b, cmp);
			if(upper->x < 0 || upper->x >= mapWidthX || upper->y < 0 || upper->y >= mapWidthY)
				continue;
			if(riverNoise->GetNoise(upper->x, upper->y) <= riverThreshold)
				continue;
			if(pointMap[upper->x][upper->y].terrain.type == "river")
				continue;
			vector<Edge*> riverEdges;
			riverEdges.push_back(edge);
			Edge* river = edge;
			river->isRiver = true;
			river->mirror->isRiver = true;
			for (;;) {
				Point* lower = std::min(river->a, river->b, cmp);
				vector<Edge*> riverCandidate;
				bool joinRiver = false;
				for (int index = 0; index < 2; ++index) {
					Polygon* riverPoly = vmap->polygons[river->parentID[index]];
					for (auto&& it : riverPoly->edges) {
						if(*std::max(it->a, it->b, cmp) == *lower){
							if(it->isRiver){
								joinRiver = true;
							}
							riverCandidate.push_back(it);
							break;
						}
					}
				}
				if(joinRiver)
					break;
				if(riverCandidate.size() == 0)
					break;
				if(riverCandidate.size() == 1){
					river = riverCandidate[0];
				}else{
					// index 0 or index 1, two choices
					int index = riverNoise->GetNoise(lower->x, lower->y) <= 0;
					river = riverCandidate[index];
				}
				riverEdges.push_back(river);
				river->isRiver = true;
			}
			double tmpRange = range;
			for (const auto& it : riverEdges) {
				int minX, maxX, minY, maxY;
				minX = mapWidthX;
				minY = mapWidthY;
				maxX = maxY = 0;
				for (Point* Edge::*jt : {&Edge::a, &Edge::b}) {
					if(minX > (it->*jt)->x)	minX = (it->*jt)->x;
					if(minY > (it->*jt)->y)	minY = (it->*jt)->y;
					if(maxX < (it->*jt)->x)	maxX = (it->*jt)->x;
					if(maxY < (it->*jt)->y)	maxY = (it->*jt)->y;
				}
				maxX = std::min(maxX + (int)ceil(tmpRange), mapWidthX);
				maxY = std::min(maxY + (int)ceil(tmpRange), mapWidthY);
				minX = std::max(minX - (int)ceil(tmpRange), 0);
				minY = std::max(minY - (int)ceil(tmpRange), 0);
				for (int x = minX; x < maxX; ++x) {
					for (int y = minY; y < maxY; ++y) {
						if(it->distance(Point(x, y)) < tmpRange){
							pointMap[x][y].terrain.type = "river";
						}
					}
				}
				tmpRange *= 1.05;
			}
		}
	}
	// water generation
	float waterLayer = (float)lua["WaterLayer"] * maxAltitude;
	for (int x = 0; x < pointMap.size(); ++x) {
		for (int y = 0; y < pointMap.at(0).size(); ++y) {
			if(pointMap[x][y].terrain.height < waterLayer){
				pointMap[x][y].terrain.type = "water";
			}
		}
	}
}

void VoronoiGen::generateBiomes()
{
	for (auto&& it : vmap->polygons) {
		sol::table t = lua.create_table();
		t["moisture"] = 0.5;
		t["height"] = (float)it->focus->terrain.height / maxAltitude;

		sol::table bio = biomes.setBiome(t);

		it->focus->terrain.type = bio["Type"];
		for (int i = 0; i < 3; ++i) {
			it->focus->terrain.color[i] = (unsigned char)bio["Color"][i + 1];
		}
	}
	for (auto&& poly : vmap->polygons) {
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
					pointMap[x][y].terrain = poly->focus->terrain;
				}
			}
		}
	}
}

int VoronoiGen::getMaxAltitude()
{
	return maxAltitude;
}

float VoronoiGen::getNoise(float x, float y)
{
	return perlinNoise->GetNoise(x + currentChunk[0] * mapWidthX, y + currentChunk[1] * mapWidthY);
}

void VoronoiGen::mamemaki(const Rectangle&& range)
{
	double threshold = (double)lua["MamemakiThreshold"];
	int mamemakiOffset = (int)lua["MamemakiOffset"];
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

VoronoiGen::Smooth::Smooth(VoronoiGen *parent):
	parent(parent)
{
}

void VoronoiGen::Smooth::loadScript(string path)
{
	interpolateM.clear();
	sol::table t = parent->lua["Smooth"]["InterpolateMatrix"];
	for (int i = 0; i < t.size(); ++i) {
		interpolateM.push_back(std::vector<double>());
		for (int j = 0; j < ((sol::table)t[i + 1]).size(); ++j) {
			interpolateM[i].push_back((double)t[i + 1][j + 1]);
		}
	}
	auto func = parent->lua["Smooth"]["Method"];
	if(func.valid()){
		scriptMethod = (sol::function)func;
	}else{
		scriptMethod = {};
	}
}

void VoronoiGen::Smooth::interpolateMethod(int iteration)
{
	if(scriptMethod){
		sol::table t = parent->lua.create_table();
		for (int i = 0; i < parent->pointMap.size(); ++i) {
			t[i + 1] = parent->lua.create_table();
			for (int j = 0; j < parent->pointMap.at(i).size(); ++j) {
				t[i + 1][j + 1] = parent->lua.create_table();
				t[i + 1][j + 1]["height"] = parent->pointMap[i][j].terrain.height;
			}
		}
		for (int iter = 0; iter < iteration; ++iter) {
			t = scriptMethod(t);
		}
		for (int i = 0; i < parent->pointMap.size(); ++i) {
			for (int j = 0; j < parent->pointMap.at(i).size(); ++j) {
				parent->pointMap[i][j].terrain.height = t[i + 1][j + 1]["height"];
			}
		}
	}else{
		for (int iter = 0; iter < iteration; ++iter) {
			vector< vector< voronoiMap::Point > > tempMap = parent->pointMap;
			for (int x = 0; x < tempMap.size(); ++x) {
				for (int y = 0; y < tempMap.at(x).size(); ++y) {
					double sum = 0, sumBase = 0;
					for (int i = 0; i < interpolateM.size(); ++i) {
						int ii = x + (i - interpolateM.size() / 2);
						if(ii < 0 || ii >= parent->pointMap.size())
							continue;
						for (int j = 0; j < interpolateM.at(i).size(); ++j) {
							int jj = y + (j - interpolateM.size() / 2);
							if(jj < 0 || jj >= parent->pointMap.at(0).size())
								continue;
							sum += parent->pointMap[ii][jj].terrain.height * interpolateM[i][j];
							sumBase += interpolateM[i][j];
						}
					}
					tempMap[x][y].terrain.height = sum / sumBase;
				}
			}
			parent->pointMap = tempMap;
		}
	}
}

VoronoiGen::Biomes::Biomes(VoronoiGen *parent):
	parent(parent)
{
}

void VoronoiGen::Biomes::loadScript(string path)
{
	parent->lua.script_file(path + "/biome.lua");
	setBiome = parent->lua["setBiome"];
}
