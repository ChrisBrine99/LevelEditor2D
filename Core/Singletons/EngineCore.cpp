#include "EngineCore.hpp"
INIT_SINGLETON_CPP(EngineCore)

#include "../Extensions/EngineCoreExt.hpp"

#define BUTTON_INDEX_INVALID			0xFFFFFFFFFFFFFFFFui64
#define ID_INVALID						0xFFFFui16

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	EngineCore class function definitions.																								//
// ------------------------------------------------------------------------------------------------------------------------------------ //

Font* EngineCore::fontSmall = nullptr;

EngineCore::EngineCore() :
	curButton(BUTTON_INDEX_INVALID),
	map(nullptr),
	x(-64i32),
	y(-64i32),
	engineExt(new EngineCoreExt(true)),
	uiButtons(),
	prevMouseX(0i32),
	prevMouseY(0i32),
	prevTileX(0i32),
	prevTileY(0i32),
	sMousePosition("(0, 0)"),
	sTilePosition("[0, 0]"),
	dragLastX(0),
	dragLastY(0)
{ // Sets the name for the application and reserves memory for the UI buttons that will be created in OnUserCreate.
	sAppName = "Turn Based RPG";
	uiButtons.reserve(10ui64);
}

bool EngineCore::OnUserCreate() {
	uint16_t _startWidth = 20ui16;
	uint16_t _startHeight = 16ui16;

	map = new Map(_startWidth, _startHeight);
	fontSmall = new Font("Resources/Graphics/Fonts/font_small.png", "Resources/Graphics/Fonts/font_small_data.fnt");

	ButtonMapWidth* _bMapWidth = new ButtonMapWidth(10i32, 10i32, 80i32, 12i32, _startWidth);
	uiButtons.push_back(_bMapWidth);

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

	olc::HWButton _middleButton = GetMouse(olc::Mouse::MIDDLE);
	olc::HWButton _leftButton = GetMouse(olc::Mouse::LEFT);
	olc::HWButton _keyControl = GetKey(olc::CTRL);
	if (_mouseX < GUI_WIDTH || _middleButton.bPressed || (_leftButton.bPressed && _keyControl.bHeld)) {
		dragLastX = GetMouseX();
		dragLastY = GetMouseY();
	}

	if (_mouseX >= GUI_WIDTH && (_middleButton.bHeld || (_leftButton.bHeld && _keyControl.bHeld))) {
		if (dragLastX != _mouseX) {
			x += dragLastX - _mouseX;
			dragLastX = _mouseX;
		}

		if (dragLastY != _mouseY) {
			y += dragLastY - _mouseY;
			dragLastY = _mouseY;
		}
		return true;
	}

	if (curButton != BUTTON_INDEX_INVALID) {
		uiButtons[curButton]->ActivationState(this);
		return true;
	}

	size_t _index = 0ui64;
	for (Button* _button : uiButtons) {
		_button->OnUserUpdate(this, _deltaTime, _mouseX, _mouseY);
		if (_button->flags & FLAG_BUTTON_SELECTED) {
			curButton = _index;
			break;
		}
		_index++;
	}
	return true;
}

bool EngineCore::OnUserRender(float_t _deltaTime) {
	Clear(COLOR_DARK_GRAY);

	map->OnUserRender(this, _deltaTime);

	olc::Pixel _backColor = COLOR_BLACK;
	_backColor.a = 188ui8;
	FillRectDecal(olc::vf2d(0.0f, 0.0f), olc::vf2d(GUI_WIDTH_F, GUI_HEIGHT_F), _backColor);
	FillRectDecal(olc::vf2d(GUI_WIDTH_F, 0.0f), olc::vf2d(1.0f, GUI_HEIGHT_F), COLOR_WHITE);

	for (Button* _button : uiButtons)
		_button->OnUserRender(this, _deltaTime);

	fontSmall->DrawString(60 - fontSmall->StringWidth("Mouse X/Y"), VIEWPORT_HEIGHT - 25, "Mouse X/Y",	COLOR_LIGHT_RED);
	fontSmall->DrawString(60 - fontSmall->StringWidth("Tile X/Y"),  VIEWPORT_HEIGHT - 15, "Tile X/Y",	COLOR_LIGHT_YELLOW);
	fontSmall->DrawString(65, VIEWPORT_HEIGHT - 25,	sMousePosition);
	fontSmall->DrawString(65, VIEWPORT_HEIGHT - 15,	sTilePosition);

	return true;
}

void EngineCore::OnTextEntryComplete(const std::string& _text) {
	std::cout << "Text Entry: " << _text << std::endl;
}

void EngineCore::UpdateMouseInfo(int32_t _x, int32_t _y) {
	// Don't update the mouse position info OR its tile info if the mouse hasn't moved between frames.
	if (_x == prevMouseX && _y == prevMouseY)
		return;
	prevMouseX = _x;
	prevMouseY = _y;

	// Update the mouse's position data on the UI to match the current coordinate offset of the map itself in the window and the mouse's
	// position on the screen applied to that offset. Then, convert those values into a string.
	int32_t _worldX		= x + _x;
	int32_t _worldY		= y + _y;
	sMousePosition		= "(" + std::to_string(_worldX) + ", " + std::to_string(_worldY) + ")";

	// Determine which tile the mouse is currently hovering over along the x axis. Fix issue with negative values by offseting by one.
	int32_t _tileX = _worldX / 16i32;
	if (_worldX < 0i32) { _tileX--; }

	// Determine which tile the mouse is currently hovering over along the y axis. Fix issue with negative values by offseting by one.
	int32_t _tileY = _worldY / 16i32;
	if (_worldY < 0i32) { _tileY--; }

	// Only bother updating the string containing the current mouse tile position if the mouse has moved to a different tile. Otherwise,
	// skip the code that converts the numerical values to a visible string for the UI.
	if (_tileX == prevTileX && _tileY == prevTileY)
		return;
	prevTileX		= _tileX;
	prevTileY		= _tileY;
	sTilePosition	= "[" + std::to_string(_tileX) + ", " + std::to_string(_tileY) + "]";
}

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Font class function definitions.																									//
// ------------------------------------------------------------------------------------------------------------------------------------ //

Font::Font(const std::string& _sprFilepath, const std::string& _dataFilepath) :
	sheet(nullptr),
	sheetDecal(nullptr),
	characters(),
	lineSpacing(0ui8)
{ // Automatically loads the sprite for the font as well as the data for the glyphs contained within the font spritesheet upon creation.
	LoadFontSprite(_sprFilepath);
	GenerateGlyphData(_dataFilepath);
}

bool Font::OnUserCreate() {
	return true;
}

bool Font::OnUserDestroy() {
	if (sheet)		{ delete sheet, sheet = nullptr; }
	if (sheetDecal) { delete sheetDecal, sheetDecal = nullptr; }

	for (auto& _data : characters)
		delete _data.second, _data.second = nullptr;
	characters.clear();

	return true;
}

void Font::DrawString(int32_t _x, int32_t _y, const std::string& _string, olc::Pixel _color) {
	EngineCore* _engine = GET_SINGLETON(EngineCore);
	int32_t _xOffset = _x;
	int32_t _yOffset = _y;
	char _curChar = 0i8;
	Glyph* _glyph = nullptr;
	size_t _length = _string.size();
	for (size_t i = 0ui64; i < _length; i++) {
		_curChar = _string.at(i);
		if (_curChar == '\n') {
			_xOffset  = _x;
			_yOffset += lineSpacing;
			continue;
		}

		_glyph = GetGlyph(_curChar);
		if (!_glyph || _curChar == ' ') { // Invalid characters are treated as spaces.
			_xOffset += characters[' ']->width;
			continue;
		}

		_engine->DrawPartialDecal(olc::vi2d(_xOffset, _yOffset + _glyph->yOffset), olc::vi2d(_glyph->width, _glyph->height), sheetDecal,
			olc::vi2d(_glyph->sprOffsetX, _glyph->sprOffsetY), olc::vi2d(_glyph->width, _glyph->height), _color);
		_xOffset += _glyph->width;
	}
}

uint32_t Font::StringWidth(const std::string& _string) {
	if (_string.empty())
		return 0ui32;

	uint8_t _character	= 0ui8;
	uint32_t _width		= 0ui32;
	uint32_t _maxWidth	= 0ui32;
	Glyph* _glyph		= nullptr;
	size_t _length		= _string.size();
	for (size_t i = 0ui64; i < _length; i++) {
		_character = _string.at(i);
		if (_character == '\n') {
			if (_width > _maxWidth)
				_maxWidth = _width;
			_width = 0ui32;
			continue;
		}

		_glyph = GetGlyph(_character);
		if (!_glyph || _character == ' ') { // Invalid characters are treated as spaces.
			_width += characters[' ']->width;
			continue;
		}

		_width += _glyph->width;
	}

	if (_width > _maxWidth)
		return _width;
	return _maxWidth;
}

uint32_t Font::StringHeight(const std::string& _string) {
	if (_string.empty())
		return 0ui32;

	uint8_t _character	= 0ui8;
	uint32_t _cHeight	= 0ui32;
	uint32_t _height	= 0ui32;
	uint32_t _maxHeight = lineSpacing;
	Glyph* _glyph		= nullptr;
	size_t _length		= _string.size();
	for (size_t i = 0ui64; i < _length; i++) {
		_character = _string.at(i);
		if (_character == '\n') {
			if (_height > lineSpacing)	{ _maxHeight += _height; }
			else						{ _maxHeight += lineSpacing; }
			_height = 0ui32;
			continue;
		}

		_glyph = GetGlyph(_character);
		if (!_glyph) { continue; }
		
		_cHeight = _glyph->height + _glyph->yOffset;
		if (_height < _cHeight)
			_height = _cHeight;
	}

	if (_height > _maxHeight)
		return _height;
	return _maxHeight;
}

Glyph* Font::GetGlyph(uint8_t _character) {
	if (characters.find(_character) == characters.end())
		return nullptr;
	return characters[_character];
}

void Font::LoadFontSprite(const std::string& _sprFilepath) {
	if (sheet || sheetDecal)
		return;

	sheet		= new olc::Sprite(_sprFilepath);
	sheetDecal	= new olc::Decal(sheet);
}

void Font::GenerateGlyphData(const std::string& _dataFilepath) {
	std::ifstream _data(_dataFilepath, std::ios::in | std::ios::binary);
	if (!_data || !_data.is_open())
		return;

	uint64_t _buffer	= 0ui64;
	uint8_t _character	= 0ui8;
	Glyph* _glyph		= nullptr;
	while (_data.read((char*)&_buffer, sizeof(_buffer))) {
		_glyph					= new Glyph();
		_glyph->sprOffsetX		= (_buffer >> 8)	& 0xFFFFui16;
		_glyph->sprOffsetY		= (_buffer >> 24)	& 0xFFFFui16;
		_glyph->width			= (_buffer >> 40)	& 0xFFui8;
		_glyph->height			= (_buffer >> 48)	& 0xFFui8;
		_glyph->yOffset			= (_buffer >> 56)	& 0xFFui8;
		_character				=  _buffer			& 0xFFui8;
		characters[_character]	= _glyph;
	}

	lineSpacing = _buffer & 0xFFui8;
	_data.close();
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
	if (_width == 0ui16 || _height == 0ui16 || width == _width && height == _height)
		return;

	// 
	bool _isHeightSmaller = _height < height;
	if (_isHeightSmaller) {
		auto _begin = tiles.begin();
		size_t _newLastIndex = size_t(width * _height);
		for (size_t i = tiles.size() - 1ui64; i >= _newLastIndex; i--)
			tiles.erase(_begin + i);
	}

	// 
	bool _isWidthSmaller = _width < width;
	if (_isWidthSmaller) {
		auto _begin			= tiles.begin();
		size_t _outCount	= size_t(width - _width);
		size_t _index		= 0ui64;
		size_t _yy			= _width;
		while (_index != tiles.size()) {
			if (_index >= _yy) {
				for (size_t i = 0ui64; i < _outCount; i++) {
					delete tiles[_index], tiles[_index] = nullptr;
					tiles.erase(_begin + _index);
				}
				_yy += _width;
				continue;
			}
			_index++;
		}
	}

	// If no value for the width or height was defined as an argument, the map will simply use its existing width/height value.
	if (_width == MAP_WIDTH_UNCHANGED) { _width = width; }
	if (_height == MAP_HEIGHT_UNCHANGED) { _height = height; }

	// 
	if (_isWidthSmaller || _isHeightSmaller) {
		GenerateNewMapTexture(_width, _height);
		return;
	}

	std::vector<Tile*> _map = std::vector<Tile*>();
	size_t _size = size_t(_width * _height);
	_map.reserve(_size);
	if (tiles.size() != 0ui64) {
		for (uint16_t yy = 0ui16; yy < _height; yy++) {
			for (uint16_t xx = 0ui16; xx < _width; xx++) {
				if (xx >= width || yy >= height) {
					_map.push_back(new Tile{ 0ui16, 0ui16, 0ui32 });
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
}

void Map::GenerateNewMapTexture(uint16_t _width, uint16_t _height) {
	if (!tilesheet || !mapTexture) // No tilesheet was loaded; testure cannot be generated.
		return;
	mapTexture->pColData.resize(size_t(_width * TILE_SIZE * _height * TILE_SIZE));
	mapTexture->width = _width * TILE_SIZE;
	mapTexture->height = _height * TILE_SIZE;

	EngineCore* _engine = GET_SINGLETON(EngineCore);
	_engine->SetDrawTarget(mapTexture);

	Tile* _tile = nullptr;
	for (int32_t yy = 0ui32; yy < _height; yy++) {
		for (int32_t xx = 0ui32; xx < _width; xx++) {
			_tile = tiles[size_t(yy * _width + xx)];
			_engine->DrawPartialSprite(int32_t(xx * TILE_SIZE), int32_t(yy * TILE_SIZE), tilesheet, _tile->xCell * TILE_SIZE, _tile->yCell * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		}
	}

	_engine->SetDrawTarget(nullptr);
	mapTextureDecal->Update();
	width = _width;
	height = _height;
}

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	Button class function definitions.																									//
// ------------------------------------------------------------------------------------------------------------------------------------ //

Button::Button(int32_t _x, int32_t _y, int32_t _width, int32_t _height, const std::string& _text, uint32_t _flags) :
	x(_x),
	y(_y),
	width(_width),
	height(_height),
	text(_text),
	textX(0i32),
	textY(0i32),
	flags(_flags)
{ // Determine the position of the text such that it is centered in the bounds of the button.
	int32_t _sWidth	= EngineCore::fontSmall->StringWidth(_text);
	int32_t _sHeight = EngineCore::fontSmall->StringHeight(_text);
	textX = _x + ((_width - _sWidth) >> 1);
	textY = _y + ((_height - _sHeight) >> 1) + 1i32;
}

void Button::OnUserUpdate(EngineCore* _engine, float_t _deltaTime, int32_t _mouseX, int32_t _mouseY) {
	flags &= ~FLAG_BUTTON_IN_BOUNDS;
	if (_mouseX >= x && _mouseY >= y && _mouseX <= x + width && _mouseY <= y + height)
		flags |= FLAG_BUTTON_IN_BOUNDS;

	if (BUTTON_IS_SELECTED || !(flags & ~(FLAG_BUTTON_VISIBLE | FLAG_BUTTON_ACTIVE)) || !BUTTON_IS_MOUSE_IN_BOUNDS)
		return;

	if (_engine->GetMouse(olc::Mouse::LEFT).bReleased) {
		OnLeftMouseClick(_engine);
		if (BUTTON_IS_SELECTABLE)
			flags |= FLAG_BUTTON_SELECTED;
		return;
	}

	if (_engine->GetMouse(olc::Mouse::RIGHT).bReleased) {
		OnRightMouseClick(_engine);
		if (BUTTON_IS_SELECTABLE)
			flags |= FLAG_BUTTON_SELECTED;
	}
}

void Button::OnUserRender(EngineCore* _engine, float_t _deltaTime) {
	if (!BUTTON_IS_VISIBLE)
		return;

	olc::Pixel _bColor = COLOR_DARK_GRAY;
	if (BUTTON_IS_SELECTED)				{ _bColor = COLOR_VERY_DARK_GREEN; }
	else if (BUTTON_IS_MOUSE_IN_BOUNDS) { _bColor = COLOR_VERY_DARK_YELLOW; }
	_engine->FillRectDecal(olc::vf2d(float_t(x), float_t(y)), olc::vf2d(float_t(width), float_t(height)), _bColor);

	if (text.empty())
		return;

	olc::Pixel _tColor = COLOR_WHITE;
	if (BUTTON_IS_SELECTED)				{ _tColor = COLOR_LIGHT_GREEN; }
	else if (BUTTON_IS_MOUSE_IN_BOUNDS) { _tColor = COLOR_YELLOW; }
	EngineCore::fontSmall->DrawString(textX, textY, text, _tColor);
}

void Button::OnLeftMouseClick(EngineCore* _engine) {}
void Button::OnRightMouseClick(EngineCore* _engine) {}

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	ButtonMapWidth function declarations.																								//
// ------------------------------------------------------------------------------------------------------------------------------------ //

ButtonMapWidth::ButtonMapWidth(int32_t _x, int32_t _y, int32_t _width, int32_t _height, uint16_t _mapWidth) :
	Button(_x, _y, _width, _height, "Map Width", FLAG_BUTTON_ACTIVE | FLAG_BUTTON_VISIBLE | FLAG_BUTTON_SELECTABLE),
	sMapWidth(std::to_string(_mapWidth))
{ // Initialize the width of the string displaying the map's width in whole pixels by calling the font's "StringWidth" function on creation.
	sMapWidthWidth = EngineCore::fontSmall->StringWidth(sMapWidth);
}

void ButtonMapWidth::OnUserUpdate(EngineCore* _engine, float_t _deltaTime, int32_t _mouseX, int32_t _mouseY) {
	if (BUTTON_IS_SELECTED) {

		return;
	}

	Button::OnUserUpdate(_engine, _deltaTime, _mouseX, _mouseY);
}

void ButtonMapWidth::OnUserRender(EngineCore* _engine, float_t _deltaTime) {
	Button::OnUserRender(_engine, _deltaTime);

	if (BUTTON_IS_SELECTED) {
		std::string _textInput = _engine->TextEntryGetString();
		if (_textInput.empty())
			return;

		std::string _substr = _textInput.substr(0ui64, 3ui64);
		int32_t _inputWidth = EngineCore::fontSmall->StringWidth(_substr);
		EngineCore::fontSmall->DrawString(GUI_WIDTH - x - _inputWidth, textY, _textInput.substr(0ui64, 3ui64), COLOR_LIGHT_RED);
		return;
	}

	EngineCore::fontSmall->DrawString(GUI_WIDTH - x - sMapWidthWidth, textY, sMapWidth, COLOR_LIGHT_RED);
}

void ButtonMapWidth::OnLeftMouseClick(EngineCore* _engine) {
	_engine->TextEntryEnable(true);
}

void ButtonMapWidth::ActivationState(EngineCore* _engine) {
	if (_engine->GetKey(olc::ESCAPE).bPressed) {
reset_activation_state:
		_engine->TextEntryEnable(false);
		_engine->curButton = BUTTON_INDEX_INVALID;
		flags &= ~FLAG_BUTTON_SELECTED;
		return;
	}

	if (_engine->GetKey(olc::ENTER).bPressed) {
		std::string _text = _engine->TextEntryGetString().substr(0ui64, 3ui64);
		if (std::find_if(_text.begin(), _text.end(), ::isdigit) == _text.end())
			goto reset_activation_state;

		sMapWidth			= _text;
		sMapWidthWidth		= EngineCore::fontSmall->StringWidth(sMapWidth);
		uint16_t _textInt	= uint16_t(std::stoi(_text));
		_engine->map->UpdateMapDimensions(_textInt);
		goto reset_activation_state;
	}
}