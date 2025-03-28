#ifndef MAIN_EDITOR_HPP
#define MAIN_EDITOR_HPP

#include "../Utils/GeneralMacros.hpp"

#include <vector>

class EngineCore;
struct TextElement;

class MainEditor {
	// Singleton Initialization (Creates Constructor/Destrcutor Declarations)
	INIT_SINGLETON_HPP(MainEditor)
public: // Main Engine Function Declarations
	bool OnUserCreate();
	bool OnUserDestroy();
	bool OnUserUpdate(float_t _deltaTime);
	bool OnUserRender(EngineCore* _engine, float_t _deltaTime);

public: // Publicly Accessible Member Variables
	std::vector<TextElement*> textElements;
};

#endif