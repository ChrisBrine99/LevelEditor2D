#ifndef MAP_MANAGER_HPP
#define MAP_MANAGER_HPP

#include "../Utils/GeneralMacros.hpp"

#include <string>
#include <vector>

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define MAP_INDEX_INVALID					0xFFFFFFFFFFFFFFFFui64

class Tile;
struct MapData;

class MapManager {
	// Singleton Initialization (Creates Constructor/Destrcutor Declarations)
	INIT_SINGLETON_HPP(MapManager)

public: // Main Engine Function Declarations
	bool OnUserUpdate(float_t _deltaTime);
	bool OnUserRender(float_t _deltaTime);

public: // Public Utility Function Declarations
	bool LoadMap(const std::string& _path, uint16_t _tilesheetID, uint16_t _id);
	void UnloadMap(uint16_t _id);

private: // Hidden Utility Function Declarations
	inline size_t FindMapWithID(uint16_t _id) {
		for (MapData& _data : loadedMaps) {
			if (_data.mapID == _id)
				return _data.mapID;
		}
		return MAP_INDEX_INVALID;
	}

public: // Publicly Member Variable Declarations
	std::vector<MapData> loadedMaps;
	size_t curMapIndex;
};

struct MapData {
	std::vector<Tile*> tiles;
	uint16_t mapID;
	uint16_t tilesheetID;
	uint16_t width;
	uint16_t height;
};

#endif