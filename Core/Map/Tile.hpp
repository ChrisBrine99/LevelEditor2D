#ifndef TILE_HPP
#define TILE_HPP

#include <cstdint>
#include <string>

class Tile {
public: // Constructor and Destructor Declaration
	Tile(const std::string& _tileset, uint16_t _index, uint16_t _xCell, uint16_t _yCell, uint32_t _flags);
	~Tile() = default;

public: // Publicly Accessible Member Variable Declarations
	const std::string tileset;
	uint16_t index;

	uint16_t xCell;
	uint16_t yCell;

	uint32_t flags;
};

#endif