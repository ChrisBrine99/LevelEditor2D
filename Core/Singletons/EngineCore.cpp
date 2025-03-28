#include "EngineCore.hpp"
INIT_SINGLETON_CPP(EngineCore)

#include "MainEditor.hpp"
#include "MapManager.hpp"
#include "../Extensions/EngineCoreExt.hpp"

EngineCore::EngineCore() :
	curButton(BUTTON_INDEX_INVALID),
	x(-64i32),
	y(-64i32),
	engineExt(new EngineCoreExt(true)),
	prevMouseX(0i32),
	prevMouseY(0i32),
	prevTileX(0i32),
	prevTileY(0i32),
	dragLastX(0),
	dragLastY(0)
{ // Sets the name for the application.
	sAppName = "Turn Based RPG";
}

bool EngineCore::OnUserCreate() {
	CALL_SINGLETON_CREATE(MapManager, OnUserCreate);
	CALL_SINGLETON_CREATE(MainEditor, OnUserCreate);
	return true;
}

bool EngineCore::OnUserDestroy() {
	CALL_SINGLETON_DESTROY(MapManager);
	CALL_SINGLETON_DESTROY(MainEditor);
	delete engineExt, engineExt = nullptr;
	return true;
}

bool EngineCore::OnUserUpdate(float_t _deltaTime) {
	return true;
}

bool EngineCore::OnUserRender(float_t _deltaTime) {
	Clear(COLOR_DARK_GRAY);

	EngineCore* _engine = this;
	CALL_SINGLETON_RENDER(MapManager);
	CALL_SINGLETON_RENDER(MainEditor);
	return true;
}

//void EngineCore::UpdateMouseInfo(int32_t _x, int32_t _y) {
//	// Don't update the mouse position info OR its tile info if the mouse hasn't moved between frames.
//	if (_x == prevMouseX && _y == prevMouseY)
//		return;
//	prevMouseX = _x;
//	prevMouseY = _y;
//
//	// Update the mouse's position data on the UI to match the current coordinate offset of the map itself in the window and the mouse's
//	// position on the screen applied to that offset. Then, convert those values into a string.
//	int32_t _worldX		= x + _x;
//	int32_t _worldY		= y + _y;
//	sMousePosition		= "(" + std::to_string(_worldX) + ", " + std::to_string(_worldY) + ")";
//
//	// Determine which tile the mouse is currently hovering over along the x axis. Fix issue with negative values by offseting by one.
//	int32_t _tileX = _worldX / 16i32;
//	if (_worldX < 0i32) { _tileX--; }
//
//	// Determine which tile the mouse is currently hovering over along the y axis. Fix issue with negative values by offseting by one.
//	int32_t _tileY = _worldY / 16i32;
//	if (_worldY < 0i32) { _tileY--; }
//
//	// Only bother updating the string containing the current mouse tile position if the mouse has moved to a different tile. Otherwise,
//	// skip the code that converts the numerical values to a visible string for the UI.
//	if (_tileX == prevTileX && _tileY == prevTileY)
//		return;
//	prevTileX		= _tileX;
//	prevTileY		= _tileY;
//	sTilePosition	= "[" + std::to_string(_tileX) + ", " + std::to_string(_tileY) + "]";
//}