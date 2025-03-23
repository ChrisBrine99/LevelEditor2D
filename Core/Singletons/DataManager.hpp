#ifndef DATA_MANAGER_HPP
#define DATA_MANAGER_HPP

#include "../Utils/GeneralMacros.hpp"
#include "../Utils/DataMacros.hpp"

#include <unordered_map>
#include <string>

namespace olc {
	class Sprite;
}

class DataManager {
	INIT_SINGLETON_HPP(DataManager)
public: // Publicly Accessible Utility Function Declarations
	void LoadSprite(const std::string& _path, uint16_t _index);
	void UnloadSprite(uint16_t _index);

public: // Getter Function Declarations
	olc::Sprite* GetSprite(uint16_t _index);

private: // Hidden Member Variable Declarations
	std::unordered_map<uint16_t, olc::Sprite*> sprites;
};

#endif