#include "Button.hpp"

#include "../TextElement.hpp"
#include "../../Singletons/EngineCore.hpp"

Button::Button(int32_t _x, int32_t _y, int32_t _width, int32_t _height, uint32_t _flags, const std::string& _text, uint8_t _hAlign, uint8_t _vAlign) :
	x(_x),
	y(_y),
	width(_width),
	height(_height),
	text(nullptr),
	flags(_flags)
{ // Create a text element with the desired alignments relative to the button's bounds.
	if (_text.empty())
		return;

	float_t _textX = float_t(_x); // Determine anchor position along the width of the button on the x axis.
	if (_hAlign == HALIGN_CENTER)		{ _textX = float_t(_x + _width) / 2.0f; }
	else if (_hAlign == HALIGN_RIGHT)	{ _textX = float_t(_x + _width); }

	float_t _textY = float_t(_y); // Determine anchor position along the height of the button on the y axis.
	if (_vAlign == VALIGN_MIDDLE)		{ _textY = float_t(_y + _height) / 2.0f; }
	else if (_vAlign == VALIGN_BOTTOM)	{ _textY = float_t(_y + _height); }

	// The color is determined by whether the mouse is hovering over the button, it was previously selected, it was inactive, and 
	// so on. So, the color is defaulted to white since the argument is required to set a custom horizontal and vertical alignment.
	text = new TextElement(_text, _textX, _textY, COLOR_WHITE, _hAlign, _vAlign);
}

Button::~Button() {
	if (text) { delete text, text == nullptr; }
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

	if (text) {
		if (BUTTON_IS_SELECTED)				{ text->color = COLOR_LIGHT_GREEN; }
		else if (BUTTON_IS_MOUSE_IN_BOUNDS) { text->color = COLOR_YELLOW; }
		else								{ text->color = COLOR_WHITE; }
		text->OnUserRender(_engine);
	}
}

void Button::OnLeftMouseClick(EngineCore* _engine) {}
void Button::OnRightMouseClick(EngineCore* _engine) {}