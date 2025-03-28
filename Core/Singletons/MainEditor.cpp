#include "MainEditor.hpp"
INIT_SINGLETON_CPP(MainEditor)

#include "EngineCore.hpp"
#include "../UI/TextElement.hpp"

MainEditor::MainEditor() :
	textElements()
{
	textElements.reserve(2ui64);
}

bool MainEditor::OnUserCreate() {
	textElements.push_back(new TextElement("(0, 0)", 88.0f, VIEWPORT_HEIGHT_F - 15.0f, COLOR_WHITE, HALIGN_LEFT, VALIGN_BOTTOM));
	textElements.push_back(new TextElement("[0, 0]", 88.0f, VIEWPORT_HEIGHT_F -  5.0f, COLOR_WHITE, HALIGN_LEFT, VALIGN_BOTTOM));
	return true;
}

bool MainEditor::OnUserDestroy() {
	for (TextElement* _text : textElements)
		delete _text, _text = nullptr;
	textElements.clear();
	return true;
}

bool MainEditor::OnUserUpdate(float_t _deltaTime) {
	return true;
}

bool MainEditor::OnUserRender(EngineCore* _engine, float_t _deltaTime) {
	for (TextElement* _text : textElements)
		_text->OnUserRender(_engine);
	return true;
}