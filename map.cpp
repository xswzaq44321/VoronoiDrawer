#include "map.h"

using namespace std;

template<unsigned int WIDTH_X, unsigned int WIDTH_Y, unsigned int HEIGHT>
Map<WIDTH_X, WIDTH_Y, HEIGHT>::Map(const std::string& folderPath):
	folderPath(folderPath)
{
}

template<unsigned int WIDTH_X, unsigned int WIDTH_Y, unsigned int HEIGHT>
Map::~Map()
{
	for (auto it : chunks) {
		for (auto iit : it->second) {
			delete iit->second;
		}
		it->second.clear();
	}
	chunks.clear();
}

template<unsigned int WIDTH_X, unsigned int WIDTH_Y, unsigned int HEIGHT>
Chunk<WIDTH_X, WIDTH_Y, HEIGHT>* Map<WIDTH_X, WIDTH_Y, HEIGHT>::getChunk(int x, int y)
{
	dir = opendir(folderPath.c_str());
	if(chunks.count(x) && chunks.at(x).count(y))
		return chunks.at(x).at(y);
	while((this->ent = readdir(this->dir)) != nullptr){
		int cx, cy;
		sscanf(this->ent->d_name, "c.%d.%d", &cx, &cy);
		if(cx == x, cy == y){
			string buffer = folderPath + "/" + this->ent->d_name;
			Chunk *bar = new Chunk(buffer);
			if(!chunks.count(x))
				chunks.at(x) = std::map<int, Chunk*>();
			chunks.at(x).at(y) = bar;
		}
	}
	return nullptr;
}

template<unsigned int WIDTH_X, unsigned int WIDTH_Y, unsigned int HEIGHT>
void Map<WIDTH_X, WIDTH_Y, HEIGHT>::setFolderPath(std::string path)
{
	folderPath = path;
	for (auto it : chunks) {
		for (auto iit : it->second) {
			delete iit->second;
		}
		it->second.clear();
	}
	chunks.clear();
}

template<unsigned int WIDTH_X, unsigned int WIDTH_Y, unsigned int HEIGHT>
Chunk<WIDTH_X, WIDTH_Y, HEIGHT>::Chunk(std::string cFilePath):
	cFilePath(cFilePath)
{
	ifstream f(cFilePath);
	string line;
	if(f.good()){
		for (int x = 0; x < WIDTH_X; ++x) {
			for (int y = 0; y < WIDTH_Y; ++y) {
				getline(f, line);
				LineReader reader(line);
				for(int h = 0; !reader.finish(); ++h){
					blocks[x][y][h] = reader.getNext();
				}
			}
		}
	}
	f.close();
}

template<unsigned int WIDTH_X, unsigned int WIDTH_Y, unsigned int HEIGHT>
Chunk<WIDTH_X, WIDTH_Y, HEIGHT>::LineReader::LineReader(string line)
{
	istringstream stream(line);
	string token;
	while(std::getline(stream, token, delimiter)){
		tokens.push_back(token);
	}
	currentIndex = 0;
}

template<unsigned int WIDTH_X, unsigned int WIDTH_Y, unsigned int HEIGHT>
bool Chunk<WIDTH_X, WIDTH_Y, HEIGHT>::LineReader::finish()
{
	return currentIndex == tokens.size();
}

template<unsigned int WIDTH_X, unsigned int WIDTH_Y, unsigned int HEIGHT>
Block &&Chunk<WIDTH_X, WIDTH_Y, HEIGHT>::LineReader::getNext()
{
	if(currentIndex == tokens.size()){
		throw out_of_range("LineReader:: No next to read");
	}

	Block rtv;
	rtv.type = tokens[currentIndex++];
	return move(rtv);
}


Block::Block()
{
	type = "air";
}

Block &Block::operator =(const Block &other)
{
	this->type = other.type;
}
