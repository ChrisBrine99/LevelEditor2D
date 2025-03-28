#include "TextElement.hpp"

#include "../Singletons/EngineCore.hpp"

TextElement::TextElement(const std::string& _text, float_t _x, float_t _y, olc::Pixel _color, uint8_t _hAlign, uint8_t _vAlign) :
	color(_color),
	position({ _x, _y }),
	alignPosition({ _x, _y }),
	textSize({ 0.0f, 0.0f }),
	hAlign(HALIGN_INVALID),
	vAlign(VALIGN_INVALID)
{ // Utilize the general SetText function to both initialize the start text for the element as well as the alignment offsets at once.
	SetText(_text, _hAlign, _vAlign);
}

void TextElement::OnUserRender(EngineCore* _engine) const {
	_engine->DrawStringDecal(alignPosition, text, color);
}

void TextElement::SetText(const std::string& _text, uint8_t _hAlign, uint8_t _vAlign) {
	text = _text;
	if (_text.empty())
		return;

	textSize.x	= EngineCore::StringWidth<float_t>(_text);
	textSize.y	= EngineCore::StringHeight<float_t>(_text);

	if (_hAlign != HALIGN_NOCHANGE) { hAlign = HALIGN_INVALID; }
	if (_vAlign != VALIGN_NOCHANGE) { vAlign = VALIGN_INVALID; }
	SetTextAlignment(_hAlign, _vAlign);
}

void TextElement::SetTextAlignment(uint8_t _hAlign, uint8_t _vAlign) {
	// Calculating the horizontal offset for the text element relative to its x origin position.
	if (_hAlign != HALIGN_NOCHANGE && _hAlign != hAlign) {
		// Determine which offset formula to use depending on which alignment type is set in the function parameter.
		if (_hAlign == HALIGN_LEFT)			{ alignPosition.x = position.x; }
		else if (_hAlign == HALIGN_CENTER)	{ alignPosition.x = position.x - (textSize.x / 2.0f); }
		else if (_hAlign == HALIGN_RIGHT)	{ alignPosition.x = position.x - textSize.x; }

		hAlign = _hAlign;
	}

	// Calculating the vertical offset for the text element relative to its y origin position.
	if (_vAlign != VALIGN_NOCHANGE && _vAlign != vAlign) {
		// Determine which offset formula to use depending on which alignment type is set in the function parameter.
		if (_vAlign == VALIGN_TOP)			{ alignPosition.y = position.y; }
		else if (_vAlign == VALIGN_MIDDLE)	{ alignPosition.y = position.y - (textSize.y / 2.0f); }
		else if (_vAlign == VALIGN_BOTTOM)	{ alignPosition.y = position.y - textSize.y; }

		vAlign = _vAlign;
	}
}