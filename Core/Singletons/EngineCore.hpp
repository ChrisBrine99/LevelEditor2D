#ifndef ENGINE_CORE_HPP
#define ENGINE_CORE_HPP

#include "../../ThirdParty/olcPixelGameEngine.hpp"
#include "../Utils/GeneralMacros.hpp"

#include <unordered_map>

class EngineCoreExt;

class EngineCore : public olc::PixelGameEngine {
	// Singleton Initialization (Creates Constructor/Destrcutor Declarations)
	INIT_SINGLETON_HPP(EngineCore)
public: // Main Engine Function Declarations
	bool OnUserCreate() override;
	bool OnUserDestroy() override;
	bool OnUserUpdate(float_t _deltaTime) override;
	bool OnUserRender(float_t _deltaTime) override;

public: // Publicly Accessible Utility Function Declarations
	//void UpdateMouseInfo(int32_t _x, int32_t _y);

	template<typename T>
	static T StringWidth(const std::string& _string) {
		T _lineWidth = (T)0;
		T _strWidth = (T)0;
		for (int8_t _char : _string) {
			if (_char >= ' ') {
				_lineWidth += (T)TEXT_WIDTH;
				continue;
			}

			switch (_char) {
			case '\n': // Shifts onto a new line; reset line width and store it should the line be wider than the previously widest line.
				if (_strWidth < _lineWidth)
					_lineWidth = _strWidth;
				_lineWidth = (T)0;
				break;
			case '\t': // A tab is equivalent to 4 characters (32 pixels).
				_lineWidth += (T)(TEXT_WIDTH * 4);
				break;
			}
		}

		// Return the value within _lineWidth if no previous width was stored (This means the string only had a single 
		// line of text and never stored a value within (_strWidth) which only occurs with multi-line strings.
		return (_strWidth == (T)0) ? _lineWidth : _strWidth;
	}

	template<typename T>
	static T StringHeight(const std::string& _string) {
		T _strHeight = (T)0;
		for (int8_t _char : _string) {
			if (_char == '\n') // Only check for newline characters since every character has a height of 8.
				_strHeight += (T)TEXT_HEIGHT;
		}
		return (_strHeight == (T)0) ? (T)TEXT_HEIGHT : _strHeight;
	}

public: // Publicly Accessible Member Variable Declarations
	size_t curButton;
	int32_t x;
	int32_t y;

private: // Hidden Member Variable Declarations
	const EngineCoreExt* engineExt;
	int32_t prevMouseX;
	int32_t prevMouseY;
	int32_t prevTileX;
	int32_t prevTileY;
	int32_t dragLastX;
	int32_t dragLastY;
};

#endif