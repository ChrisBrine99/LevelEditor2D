#include "MapManager.hpp"
INIT_SINGLETON_CPP(MapManager)

#include "../Singletons/EngineCore.hpp"

MapManager::MapManager() :
	mapSprite(),
	tilesheet(nullptr),
	tiles(),
	id(ID_INVALID),
	width(0ui16),
	height(0ui16)
{ // Reserve enough memory to store 4096 tiles--enough to account for a 64 by 64 tile map or equivalent.
	tiles.reserve(4096ui64);
}

bool MapManager::OnUserCreate() {
	tilesheet = new olc::Sprite("Resources/Graphics/background_tiles.png");

	uint16_t _startWidth = 16ui16;
	uint16_t _startHeight = 16ui16;

	mapSprite.Create(_startWidth, _startHeight);
	UpdateMapDimensions(_startWidth, _startHeight);
	return true;
}

bool MapManager::OnUserDestroy() {
	delete tilesheet, tilesheet = nullptr;
	tiles.clear();
	return true;
}

bool MapManager::OnUserRender(EngineCore* _engine, float_t _deltaTime) const {
	_engine->DrawDecal(olc::vf2d(float_t(-_engine->x), float_t(-_engine->y)), mapSprite.Decal());
	return true;
}

void MapManager::UpdateMapDimensions(uint16_t _width, uint16_t _height) {
	if (_width == 0ui16 || _height == 0ui16 || width == _width && height == _height)
		return;

	// 
	bool _isHeightSmaller = _height < height;
	if (_isHeightSmaller) {
		auto _begin		= tiles.begin();
		size_t _newEnd	= size_t(width * _height);
		for (size_t i = tiles.size() - 1ui64; i >= _newEnd; i--)
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
				for (size_t i = 0ui64; i < _outCount; i++)
					tiles.erase(_begin + _index);
				_yy += _width;
				continue;
			}
			_index++;
		}
	}

	// If no value for the width or height was defined as an argument, the map will simply use its existing width/height value.
	if (_width == MAP_WIDTH_UNCHANGED)		{ _width = width; }
	if (_height == MAP_HEIGHT_UNCHANGED)	{ _height = height; }

	// 
	if (_isWidthSmaller || _isHeightSmaller) {
		GenerateNewMapTexture(_width, _height);
		return;
	}

	std::vector<Tile> _map	= std::vector<Tile>();
	size_t _size			= size_t(_width * _height);
	_map.reserve(_size);
	if (tiles.size() != 0ui64) {
		for (uint16_t yy = 0ui16; yy < _height; yy++) {
			for (uint16_t xx = 0ui16; xx < _width; xx++) {
				if (xx >= width || yy >= height) {
					_map.push_back({ 0ui16, 0ui16, 0ui32 });
					continue;
				}
				_map.push_back(tiles[size_t(yy * width + xx)]);
			}
		}
		tiles.clear();
	} else {
		for (size_t i = 0ui64; i < _size; i++)
			_map.push_back({ 0ui16, 0ui16, 0ui32 });
	}

	tiles.swap(_map);
	GenerateNewMapTexture(_width, _height);
}

void MapManager::GenerateNewMapTexture(uint16_t _width, uint16_t _height) {
	if (!tilesheet || !mapSprite.Sprite()) // No tilesheet was loaded; testure cannot be generated.
		return;
	width	= _width;
	height	= _height;

	olc::Sprite* _sprite = mapSprite.Sprite();
	_sprite->width			= _width * TILE_SIZE;
	_sprite->height			= _height * TILE_SIZE;
	_sprite->pColData.resize(size_t(_width * TILE_SIZE * _height * TILE_SIZE));

	EngineCore* _engine = GET_SINGLETON(EngineCore);
	_engine->SetDrawTarget(_sprite);

	Tile* _tile = nullptr;
	for (int32_t yy = 0ui32; yy < _height; yy++) {
		for (int32_t xx = 0ui32; xx < _width; xx++) {
			_tile = &tiles[size_t(yy * _width + xx)];
			_engine->DrawPartialSprite(int32_t(xx * TILE_SIZE), int32_t(yy * TILE_SIZE), tilesheet, _tile->xCell * TILE_SIZE, _tile->yCell * TILE_SIZE, TILE_SIZE, TILE_SIZE);
		}
	}

	_engine->SetDrawTarget(nullptr);
	mapSprite.Decal()->Update();
}