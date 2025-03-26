#ifndef ENGINE_CORE_HPP
#define ENGINE_CORE_HPP

#include "../../ThirdParty/olcPixelGameEngine.hpp"
#include "../Utils/GeneralMacros.hpp"

#include <unordered_map>

class EngineCoreExt;
class Map;
class Font;
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
	void OnTextEntryComplete(const std::string& _text) override;

public: // Publicly Accessible Utility Function Declarations
	void UpdateMouseInfo(int32_t _x, int32_t _y);

public: // Publicly Accessible Member Variable Declarations
	size_t curButton;
	Map* map;

	static Font* fontSmall;

	int32_t x;
	int32_t y;

private: // Hidden Member Variable Declarations
	const EngineCoreExt* engineExt;
	std::vector<Button*> uiButtons;

	int32_t prevMouseX;
	int32_t prevMouseY;
	int32_t prevTileX;
	int32_t prevTileY;

	std::string sMousePosition;
	std::string sTilePosition;

	int32_t dragLastX;
	int32_t dragLastY;
};

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Font class decalaration (Stores data for the glyphs and graphical elements that make up the font).									//
// ------------------------------------------------------------------------------------------------------------------------------------ //

struct Glyph;
struct TextRenderable;

class Font {
public: // Constructor/Destructor Declarations
	Font(const std::string& _sprFilepath, const std::string& _dataFilepath);
	~Font() = default;

public: // Main Engine Function Declarations
	bool OnUserCreate();
	bool OnUserDestroy();

public: // Publicly Accessible Utility Function Decalatations
	void DrawString(int32_t _x, int32_t _y, const std::string& _string, olc::Pixel _color = COLOR_WHITE);
	uint32_t StringWidth(const std::string& _string);
	uint32_t StringHeight(const std::string& _string);

	Glyph* GetGlyph(uint8_t _character);

private: // Hidden Utility Function Declarations
	void LoadFontSprite(const std::string& _sprFilepath);
	void GenerateGlyphData(const std::string& _dataFilepath);

public: // Hidden Member Variable Declarations
	olc::Sprite* sheet;
	olc::Decal* sheetDecal;
	std::unordered_map<uint8_t, Glyph*> characters;
	uint8_t lineSpacing;
};

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Glyph struct declaration (Stores data about a letter/symbol within a given font).													//
// ------------------------------------------------------------------------------------------------------------------------------------ //

struct Glyph {
	uint16_t	sprOffsetX;
	uint16_t	sprOffsetY;
	uint8_t		width;
	uint8_t		height;
	int8_t		yOffset;

public: // Default Constructor Definition
	Glyph() :
		sprOffsetX(0ui16),
		sprOffsetY(0ui16),
		width(0ui8),
		height(0ui8),
		yOffset(0ui8)
	{}
};

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Map class declaration. Responsible for managing all the characteristics of the map for the editor.									//
// ------------------------------------------------------------------------------------------------------------------------------------ //

struct Tile;

class Map {
public: // Constructor/Destructor Declarations
	Map(uint16_t _width, uint16_t _height);
	~Map() = default;

public: // Main Engine Function Declarations
	void OnUserDestroy();
	void OnUserRender(EngineCore* _engine, float_t _deltaTime) const;

public: // Publicly Accessible Utility Function Declarations
	void UpdateMapDimensions(uint16_t _width = MAP_WIDTH_UNCHANGED, uint16_t _height = MAP_HEIGHT_UNCHANGED);

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
	Button(int32_t _x, int32_t _y, int32_t _width, int32_t _height, const std::string& _text, 
		uint32_t _flags = FLAG_BUTTON_VISIBLE | FLAG_BUTTON_ACTIVE);
	~Button() = default;

public: // Main Engine Function Declarations
	virtual void OnUserUpdate(EngineCore* _engine, float_t _deltaTime, int32_t _mouseX, int32_t _mouseY);
	virtual void OnUserRender(EngineCore* _engine, float_t _deltaTime);

public: // Publicly Accessible Utility Function Declarations
	virtual void OnLeftMouseClick(EngineCore* _engine);
	virtual void OnRightMouseClick(EngineCore* _engine);
	virtual void ActivationState(EngineCore* _engine) = 0;

public: // Publicly Accessible Member Variable Declarations
	int32_t x;
	int32_t y;
	int32_t width;
	int32_t height;

	const std::string text;
	int32_t textX;
	int32_t textY;

	uint32_t flags;
};

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	ButtonMapWidth class declaration (Inherits from basic Button class). ALlows user to change the map's current width in tiles.		//
// ------------------------------------------------------------------------------------------------------------------------------------ //

class ButtonMapWidth : public Button {
public: // Constructor/Destructor Declarations
	ButtonMapWidth(int32_t _x, int32_t _y, int32_t _width, int32_t _height, uint16_t _mapWidth);
	~ButtonMapWidth() = default;

public: // Main Engine Function Declarations
	void OnUserUpdate(EngineCore* _engine, float_t _deltaTime, int32_t _mouseX, int32_t _mouseY) override;
	void OnUserRender(EngineCore* _engine, float_t _deltaTime) override;

public: // Publicly Accessible Utility Function Override
	void OnLeftMouseClick(EngineCore* _engine) override;
	void ActivationState(EngineCore* _engine) override;

public: // Publicly Accessible Member Variables
	std::string sMapWidth;
	uint32_t sMapWidthWidth;
};

#endif