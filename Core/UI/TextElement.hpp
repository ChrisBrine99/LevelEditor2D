#ifndef TEXT_ELEMENT_HPP
#define TEXT_ELEMENT_HPP

#include "../../ThirdParty/olcPixelGameEngine.hpp"
#include "../Utils/GeneralMacros.hpp"

class EngineCore;

struct TextElement {
public: // Publicly Accessible Member Variable Declarations
	olc::Pixel	color;

private: // Hidden Member Variable Declarations
	std::string	text;
	olc::vf2d	position;
	olc::vf2d	alignPosition;
	olc::vf2d	textSize;
	uint8_t		hAlign;
	uint8_t		vAlign;

public: // Constructor/Destructor Declarations
	TextElement(const std::string& _string, float_t _x, float_t _y, olc::Pixel _color = COLOR_WHITE, uint8_t _hAlign = HALIGN_LEFT, uint8_t _vAlign = VALIGN_TOP);
	TextElement(const TextElement& _other) = delete;
	~TextElement() = default;

public: // Main Engine Function Declarations
	void OnUserRender(EngineCore* _engine) const;

public: // Publicly Accessible Utility Function Declarations
	void SetText(const std::string& _text, uint8_t _hAlign = VALIGN_NOCHANGE, uint8_t _vAlign = VALIGN_NOCHANGE);
	void SetTextAlignment(uint8_t _hAlign = HALIGN_NOCHANGE, uint8_t _vAlign = VALIGN_NOCHANGE);
};

#endif