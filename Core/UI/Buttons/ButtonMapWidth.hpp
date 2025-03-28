#ifndef BUTTON_MAP_WIDTH_HPP
#define BUTTON_MAP_WIDTH_HPP

#include "Button.hpp"

class ButtonMapWidth : public Button {
public: // Constructor/Destructor Declarations
	ButtonMapWidth(int32_t _x, int32_t _y, int32_t _width, int32_t _height, uint16_t _mapWidth);
	~ButtonMapWidth();

public: // Main Engine Function Declarations
	void OnUserUpdate(EngineCore* _engine, float_t _deltaTime, int32_t _mouseX, int32_t _mouseY) override;
	void OnUserRender(EngineCore* _engine, float_t _deltaTime) override;

public: // Publicly Accessible Utility Function Override
	void OnLeftMouseClick(EngineCore* _engine) override;
	void ActivationState(EngineCore* _engine) override;

public: // Publicly Accessible Member Variables
	TextElement* sMapWidth;
};

#endif