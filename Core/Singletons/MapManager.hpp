#ifndef MAP_MANAGER_HPP
#define MAP_MANAGER_HPP

#include "../Utils/GeneralMacros.hpp"
#include "../../ThirdParty/olcPixelGameEngine.hpp"

class EngineCore;
struct Tile;

class MapManager {
	// Singleton Initialization (Creates Constructor/Destrcutor Declarations)
	INIT_SINGLETON_HPP(MapManager)
public: // Main Engine Function Declarations
	bool OnUserCreate();
	bool OnUserDestroy();
	bool OnUserRender(EngineCore* _engine, float_t _deltaTime) const;

public: // Publicly Accessible Utility Function Declarations
	void UpdateMapDimensions(uint16_t _width = MAP_WIDTH_UNCHANGED, uint16_t _height = MAP_HEIGHT_UNCHANGED);

private: // Hidden Utility Function Declarations
	void GenerateNewMapTexture(uint16_t _width, uint16_t _height);

public: // Publicly Accessible Member Variable Declarations
	olc::Renderable		mapSprite;
	olc::Sprite*		tilesheet;
	std::vector<Tile>	tiles;
	uint16_t id;
	uint16_t width;
	uint16_t height;
};

struct Tile {
	uint16_t xCell;
	uint16_t yCell;
	uint32_t flags;
};

#endif