#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "../../Utils/GeneralMacros.hpp"

#include <cstdint>
#include <cmath>
#include <string>

class EngineCore;
struct TextElement;

class Button {
public: // Constructor/Destructor Declarations
	Button(int32_t _x, int32_t _y, int32_t _width, int32_t _height, uint32_t _flags = FLAG_BUTTON_VISIBLE | FLAG_BUTTON_ACTIVE, 
		const std::string& _text = "", uint8_t _hAlign = HALIGN_LEFT, uint8_t _vAlign = VALIGN_TOP);
	Button(const Button& _other) = delete;
	~Button();

public: // Main Engine Function Declarations
	virtual void OnUserUpdate(EngineCore* _engine, float_t _deltaTime, int32_t _mouseX, int32_t _mouseY);
	virtual void OnUserRender(EngineCore* _engine, float_t _deltaTime);

public: // Publicly Accessible Utility Function Declarations
	virtual void OnLeftMouseClick(EngineCore* _engine);
	virtual void OnRightMouseClick(EngineCore* _engine);
	virtual void ActivationState(EngineCore* _engine) = 0;

public: // Publicly Accessible Member Variable Declarations
	TextElement*	text;
	int32_t			x;
	int32_t			y;
	int32_t			width;
	int32_t			height;
	uint32_t		flags;
};

#endif