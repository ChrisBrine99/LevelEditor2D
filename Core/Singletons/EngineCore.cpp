#include "EngineCore.hpp"
INIT_SINGLETON_CPP(EngineCore)

#include "../Extensions/EngineCoreExt.hpp"

#define BUTTON_INDEX_INVALID			0xFFFFFFFFFFFFFFFFui64
#define ID_INVALID						0xFFFFui16

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	EngineCore class function definitions.																								//
// ------------------------------------------------------------------------------------------------------------------------------------ //

EngineCore::EngineCore() :
	curButton(BUTTON_INDEX_INVALID),
	map(nullptr),
	x(-64),
	y(-64),
	engineExt(new EngineCoreExt(true)),
	uiButtons(),
	prevMouseX(0),
	prevMouseY(0),
	sMousePosition(""),
	sTilePosition(""),
	sTilePositionWidth(0u),
	dragLastX(0),
	dragLastY(0)
{ // Sets the name for the application and reserves memory for the UI buttons that will be created in OnUserCreate.
	sAppName = "Turn Based RPG";
	uiButtons.reserve(10ui64);
}

bool EngineCore::OnUserCreate() {
	map = new Map(16ui16, 16ui16);

	return true;
}

bool EngineCore::OnUserDestroy() {
	if (map) {
		map->OnUserDestroy();
		delete map, map = nullptr;
	}

	for (Button* _button : uiButtons)
		delete _button, _button = nullptr;
	uiButtons.clear();

	delete engineExt, engineExt = nullptr;
	return true;
}

bool EngineCore::OnUserUpdate(float_t _deltaTime) {
	int32_t _mouseX = GetMouseX();
	int32_t _mouseY = GetMouseY();

	if (curButton != BUTTON_INDEX_INVALID) {
		uiButtons[curButton]->ActivationState(this, _deltaTime);
		return true;
	}

	olc::HWButton _middleButton = GetMouse(olc::Mouse::MIDDLE);
	if (_middleButton.bPressed) {
		dragLastX = GetMouseX();
		dragLastY = GetMouseY();
	}

	if (_middleButton.bHeld) {
		int32_t _mouseX = GetMouseX();
		if (dragLastX  != _mouseX) {
			x		   += dragLastX - _mouseX;
			dragLastX	= _mouseX;
		}

		int32_t _mouseY = GetMouseY();
		if (dragLastY  != _mouseY) {
			y		   += dragLastY - _mouseY;
			dragLastY	= _mouseY;
		}
		return true;
	}

	for (Button* _button : uiButtons)
		_button->OnUserUpdate(this, _deltaTime, _mouseX, _mouseY);
	return true;
}

bool EngineCore::OnUserRender(float_t _deltaTime) {
	Clear(olc::BLACK);

	map->OnUserRender(this, _deltaTime);

	for (Button* _button : uiButtons)
		_button->OnUserRender(this, _deltaTime, olc::WHITE);

	DrawStringDecal(olc::vi2d(0,0), sMousePosition);
	DrawStringDecal(olc::vi2d(VIEWPORT_WIDTH - sTilePositionWidth,0), sTilePosition);

	return true;
}

void EngineCore::UpdateMouseInfo(int32_t _x, int32_t _y) {
	if (_x == prevMouseX && _y == prevMouseY)
		return;

	prevMouseX = _x;
	prevMouseY = _y;

	int32_t _worldX		= x + _x;
	int32_t _worldY		= y + _y;
	sMousePosition		= "(" + std::to_string(_worldX) + ", " + std::to_string(_worldY) + ")";
	sTilePosition		= "[" + std::to_string(int32_t(std::floor(_worldX / 16.0f))) + ", " + std::to_string(int32_t(std::floor(_worldY / 16.0f))) + "]";
	sTilePositionWidth	= uint32_t(8 * sTilePosition.length());
}

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Map class function definitions.																										//
// ------------------------------------------------------------------------------------------------------------------------------------ //

Map::Map(uint16_t _width, uint16_t _height) :
	mapTexture(nullptr),
	mapTextureDecal(nullptr),
	tiles(),
	id(ID_INVALID),
	width(0ui16),
	height(0ui16)
{ // Reserve enough memory for the starting dimensions of the map. Then create a bunch of empty tiles to fill that map.
	tilesheet		= new olc::Sprite("Resources/Graphics/background_tiles.png");
	mapTexture		= new olc::Sprite(uint32_t(_width * TILE_SIZE), uint32_t(_height * TILE_SIZE));
	mapTextureDecal = new olc::Decal(mapTexture);
	UpdateMapDimensions(_width, _height);
}

void Map::OnUserDestroy() {
	delete mapTextureDecal, mapTextureDecal = nullptr;
	delete mapTexture, mapTexture = nullptr;
	delete tilesheet, tilesheet = nullptr;

	for (Tile* _tile : tiles)
		delete _tile, _tile = nullptr;
	tiles.clear();
}

void Map::OnUserRender(EngineCore* _engine, float_t _deltaTime) const {
	if (mapTextureDecal->sprite)
		_engine->DrawDecal(olc::vf2d(float_t(-_engine->x), float_t(-_engine->y)), mapTextureDecal);
}

void Map::UpdateMapDimensions(uint16_t _width, uint16_t _height) {
	if (width == _width && height == _height)
		return;

	size_t _size			= size_t(_width * _height);
	std::vector<Tile*> _map = std::vector<Tile*>();
	_map.reserve(_size);

	if (tiles.size() != 0ui64) {
		for (uint16_t yy = 0ui16; yy < _width; yy++) {
			for (uint16_t xx = 0ui16; xx < _height; xx++) {
				if (xx > width || yy > height) {
					_map.push_back(new Tile());
					continue;
				}
				_map.push_back(tiles[size_t(yy * width + xx)]);
			}
		}
		tiles.clear();
	} else {
		for (size_t i = 0ui64; i < _size; i++)
			_map.push_back(new Tile{ 0ui16, 0ui16, 0ui32 });
	}

	tiles.swap(_map);
	GenerateNewMapTexture(_width, _height);
	width = _width;
	height = _height;
}

void Map::GenerateNewMapTexture(uint16_t _width, uint16_t _height) {
	if (!tilesheet || !mapTexture) // No tilesheet was loaded; testure cannot be generated.
		return;
	mapTexture->pColData.resize(size_t(_width * TILE_SIZE * _height * TILE_SIZE));


	EngineCore* _engine = GET_SINGLETON(EngineCore);
	_engine->SetDrawTarget(mapTexture);

	Tile* _tile = nullptr;
	for (int32_t yy = 0ui32; yy < _height; yy++) {
		for (int32_t xx = 0ui32; xx < _width; xx++) {
			_tile = tiles[size_t(yy * _width + xx)];
			_engine->DrawPartialSprite(int32_t(xx * TILE_SIZE), int32_t(yy * TILE_SIZE), tilesheet, _tile->xCell * TILE_SIZE, _tile->xCell * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		}
	}

	_engine->SetDrawTarget(nullptr);
	mapTextureDecal->Update();
}

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Button class function definitions.																									//
// ------------------------------------------------------------------------------------------------------------------------------------ //

Button::Button(int32_t _x, int32_t _y, int32_t _width, int32_t _height) :
	x(_x),
	y(_y),
	width(_width),
	height(_height),
	text("")
{}

void Button::OnUserUpdate(EngineCore* _engine, float_t _deltaTime, int32_t _mouseX, int32_t _mouseY) {
	if (_mouseX < x || _mouseY < y || _mouseX > x + width || _mouseY > y + height)
		return;

	if (_engine->GetMouse(olc::Mouse::LEFT).bReleased) {
		OnLeftMouseClick(_engine);
		return;
	}

	if (_engine->GetMouse(olc::Mouse::RIGHT).bReleased)
		OnRightMouseClick(_engine);
}

void Button::OnUserRender(EngineCore* _engine, float_t _deltaTime, olc::Pixel _color) {
}

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	ButtonMapWidth function declarations.																								//
// ------------------------------------------------------------------------------------------------------------------------------------ //

ButtonMapWidth::ButtonMapWidth(int32_t _x, int32_t _y, int32_t _width, int32_t _height) :
	Button(_x, _y, _width, _height)
{ // Default the value displayed to the value 0 since no map has been initialized.
	text = "0";
}

void ButtonMapWidth::OnUserUpdate(EngineCore* _engine, float_t _deltaTime, int32_t _mouseX, int32_t _mouseY) {
	Button::OnUserUpdate(_engine, _deltaTime, _mouseX, _mouseY);
}

void ButtonMapWidth::OnLeftMouseClick(EngineCore* _engine) {
	_engine->TextEntryEnable(true, text);
}

void ButtonMapWidth::ActivationState(EngineCore* _engine, float_t _deltaTime) {

}