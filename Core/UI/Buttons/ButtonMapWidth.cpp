#include "ButtonMapWidth.hpp"

#include "../../Singletons/EngineCore.hpp"
#include "../../Singletons/MapManager.hpp"
#include "../TextElement.hpp"

ButtonMapWidth::ButtonMapWidth(int32_t _x, int32_t _y, int32_t _width, int32_t _height, uint16_t _mapWidth) :
	Button(_x, _y, _width, _height, FLAG_BUTTON_ACTIVE | FLAG_BUTTON_VISIBLE | FLAG_BUTTON_SELECTABLE, "Width (In Tiles)"),
	sMapWidth(new TextElement(std::to_string(_mapWidth), _x + _width + (TEXT_WIDTH_F * 4.0f), float_t(_y), COLOR_LIGHT_RED, HALIGN_RIGHT))
{}

ButtonMapWidth::~ButtonMapWidth() {
	delete sMapWidth, sMapWidth = nullptr;
	Button::~Button();
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
		//int32_t _inputWidth = EngineCore::fontSmall->StringWidth(_substr);
		//EngineCore::fontSmall->DrawString(GUI_WIDTH - x - _inputWidth, textY, _textInput.substr(0ui64, 3ui64), COLOR_LIGHT_RED);
		return;
	}

	//EngineCore::fontSmall->DrawString(GUI_WIDTH - x - sMapWidthWidth, textY, sMapWidth, COLOR_LIGHT_RED);
}

void ButtonMapWidth::OnLeftMouseClick(EngineCore* _engine) {
	_engine->TextEntryEnable(true);
}

void ButtonMapWidth::ActivationState(EngineCore* _engine) {
	if (_engine->GetKey(olc::ESCAPE).bPressed) {
	reset_activation_state:
		_engine->curButton = BUTTON_INDEX_INVALID;
		flags &= ~FLAG_BUTTON_SELECTED;
		return;
	}

	if (_engine->GetKey(olc::ENTER).bPressed) {
		//if (std::find_if(_text.begin(), _text.end(), ::isdigit) == _text.end())
		//	goto reset_activation_state;

		//sMapWidth->SetText(_text);
		//GET_SINGLETON(MapManager)->UpdateMapDimensions(uint16_t(std::stoi(_text)));
		//goto reset_activation_state;
	}
}