#include "Tile.hpp"

#include "../../ThirdParty/olcPixelGameEngine.hpp"

Tile::Tile(const std::string& _tileset, uint16_t _index, uint16_t _xCell, uint16_t _yCell, uint32_t _flags) :
	tileset(_tileset),
	index(_index),
	xCell(_xCell),
	yCell(_yCell),
	flags(_flags)
{ // Attempt to get the tile's position within the tileset relative to the index value provided. Otherwise default to the 0th tile AKA the error tile.
	
}