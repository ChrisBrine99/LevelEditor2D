#include "DataManager.hpp"
INIT_SINGLETON_CPP(DataManager)

#include "../../ThirdParty/olcPixelGameEngine.hpp"

DataManager::DataManager() :
	sprites()
{ // Reserve a bit of memory for the data this singleton will store.
	sprites.reserve(SPRDATA_RESERVE_AMOUNT);
}

void DataManager::LoadSprite(const std::string& _path, uint16_t _index) {

}

void DataManager::UnloadSprite(uint16_t _index) {

}

olc::Sprite* DataManager::GetSprite(uint16_t _index) {
	return nullptr;
}