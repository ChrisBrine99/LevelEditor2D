#include "MapManager.hpp"
INIT_SINGLETON_CPP(MapManager)

#include "../Map/Tile.hpp"
#include "../Utils/DataMacros.hpp"
#include "../../ThirdParty/olcPixelGameEngine.hpp"

MapManager::MapManager() :
	loadedMaps(),
	curMapIndex(0ui64)
{ // Reserve enough space for 8 maps to be loaded simultaneously.
	loadedMaps.reserve(8ui64);
}

bool MapManager::OnUserUpdate(float_t _deltaTime) {
	return true;
}

bool MapManager::OnUserRender(float_t _deltaTime) {
	return true;
}

bool MapManager::LoadMap(const std::string& _path, uint16_t _tilesheetID, uint16_t _id) {


	return true;
}

void MapManager::UnloadMap(uint16_t _id) {

}