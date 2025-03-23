#ifndef ENGINE_CORE_HPP
#define ENGINE_CORE_HPP

#include "../../ThirdParty/olcPixelGameEngine.hpp"
#include "../Utils/GeneralMacros.hpp"

class EngineCoreExt;
class Map;
class Button;

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	EngineCore class declaration (Inherited from the olc::PixelGameEngine base class that handles basic engine logic).					//
// ------------------------------------------------------------------------------------------------------------------------------------ //

class EngineCore : public olc::PixelGameEngine {
	// Singleton Initialization (Creates Constructor/Destrcutor Declarations)
	INIT_SINGLETON_HPP(EngineCore)
public: // Main Engine Function Declarations
	bool OnUserCreate() override;
	bool OnUserDestroy() override;
	bool OnUserUpdate(float_t _deltaTime) override;
	bool OnUserRender(float_t _deltaTime) override;

public: // Publicly Accessible Utility Function Declarations
	void UpdateMouseInfo(int32_t _x, int32_t _y);

public: // Publicly Accessible Member Variable Declarations
	size_t curButton;
	Map* map;

	int32_t x;
	int32_t y;

private: // Hidden Member Variable Declarations
	const EngineCoreExt* engineExt;
	std::vector<Button*> uiButtons;

	int32_t prevMouseX;
	int32_t prevMouseY;

	std::string sMousePosition;
	std::string sTilePosition;
	uint32_t sTilePositionWidth;

	int32_t dragLastX;
	int32_t dragLastY;
};

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Map class declaration. Responsible for managing all the characteristics of the map for the editor.									//
// ------------------------------------------------------------------------------------------------------------------------------------ //

struct Tile;

class Map {
public: // Constructor/Desctructor Declarations
	Map(uint16_t _width, uint16_t _height);
	~Map() = default;

public: // Main Engine Function Declarations
	void OnUserDestroy();
	void OnUserRender(EngineCore* _engine, float_t _deltaTime) const;

public: // Publicly Accessible Utility Function Declarations
	void UpdateMapDimensions(uint16_t _width, uint16_t _height);

private: // Hidden Utility Function Declarations
	void GenerateNewMapTexture(uint16_t _width, uint16_t _height);

public: // Publicly Accessible Member Variable Declarations
	olc::Sprite* mapTexture;
	olc::Decal* mapTextureDecal;
	olc::Sprite* tilesheet;
	std::vector<Tile*> tiles;
	uint16_t id;
	uint16_t width;
	uint16_t height;
};

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Tile struct declaration. Responsible for storing information about a given tile within the current map.								//
// ------------------------------------------------------------------------------------------------------------------------------------ //

struct Tile {
	uint16_t xCell;
	uint16_t yCell;
	uint32_t flags;
};

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Button class declaration. Generic class that handles mouse hover logic and interaction.												//
// ------------------------------------------------------------------------------------------------------------------------------------ //

class Button {
public: // Constructor/Destructor Declarations
	Button(int32_t _x, int32_t _y, int32_t _width, int32_t _height);
	~Button() = default;

public: // Main Engine Function Declarations
	virtual void OnUserUpdate(EngineCore* _engine, float_t _deltaTime, int32_t _mouseX, int32_t _mouseY);
	virtual void OnUserRender(EngineCore* _engine, float_t _deltaTime, olc::Pixel _color);

public: // Publicly Accessible Utility Function Declarations
	virtual void OnLeftMouseClick(EngineCore* _engine) = 0;
	virtual void OnRightMouseClick(EngineCore* _engine) = 0;
	virtual void ActivationState(EngineCore* _engine, float_t _deltaTime) = 0;

public: // Publicly Accessible Member Variable Declarations
	int32_t x;
	int32_t y;
	int32_t width;
	int32_t height;
	std::string text;
};

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	ButtonMapWidth class declaration (Inherits from basic Button class). ALlows user to change the map's current width in tiles.		//
// ------------------------------------------------------------------------------------------------------------------------------------ //

class ButtonMapWidth : public Button {
public: // Constructor/Destructor Declarations
	ButtonMapWidth(int32_t _x, int32_t _y, int32_t _width, int32_t _height);
	~ButtonMapWidth() = default;

public: // Main Engine Function Declarations
	void OnUserUpdate(EngineCore* _engine, float_t _deltaTime, int32_t _mouseX, int32_t _mouseY) override;

public: // Publicly Accessible Utility Function Override
	void OnLeftMouseClick(EngineCore* _engine) override;
	void ActivationState(EngineCore* _engine, float_t _deltaTime) override;
};

#endif