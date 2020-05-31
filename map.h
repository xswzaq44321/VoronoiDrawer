#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <utility>
#include <map>

class Block
{
public:
	Block();

	std::string type;

	Block& operator =(const Block& other);
};


template<unsigned int WIDTH_X, unsigned int WIDTH_Y, unsigned int HEIGHT = 1>
class Chunk
{
public:
	Chunk(std::string cFilePath);
private:
	Block blocks[WIDTH_X][WIDTH_Y][HEIGHT];
	std::string cFilePath;

	class LineReader
	{
	public:
		LineReader(std::string line);
		bool finish();
		Block&& getNext();
	private:
		std::vector<std::string> tokens;
		const char delimiter = '|';
		int currentIndex;
	};
};

template<unsigned int WIDTH_X, unsigned int WIDTH_Y, unsigned int HEIGHT = 1>
class Map
{
using Chunk = Chunk<WIDTH_X, WIDTH_Y, HEIGHT>;
public:
	Map(const std::string& folderPath);
	~Map();

	Chunk* getChunk(int x, int y);
	void setFolderPath(std::string path);

private:
	std::string folderPath;
	DIR* dir;
	dirent *ent;
	std::map<int, std::map<int, Chunk*>> chunks;
};

#endif // MAP_H
