#pragma once

#include <string>
#include <vector>

struct FlareMapEntity {
	std::string type;
	float x;
	float y;
};

class FlareMap {
	public:
		FlareMap();
		~FlareMap();
	
		void Load(const std::string fileName);

		int mapWidth;
		int mapHeight;
		unsigned int **mapData;
		std::vector<FlareMapEntity> entities;
	
	private:
	
		bool ReadHeader(std::ifstream &stream);
		bool ReadLayerData(std::ifstream &stream);
		bool ReadEntityData(std::ifstream &stream);
	
};
