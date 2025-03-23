#include "EngineCoreExt.hpp"
#include "../Singletons/EngineCore.hpp"

EngineCoreExt::EngineCoreExt(bool _hook) :
	PGEX(_hook)
{}

bool EngineCoreExt::OnBeforeUserCreate() {
	return true;
}

bool EngineCoreExt::OnAfterUserCreate() {
	return true;
}

bool EngineCoreExt::OnBeforeUserUpdate(float_t _deltaTime) {
	return true;
}

bool EngineCoreExt::OnAfterUserUpdate(float_t _deltaTime) {
	EngineCore* _engine = GET_SINGLETON(EngineCore);
	_engine->UpdateMouseInfo(_engine->GetMouseX(), _engine->GetMouseY());
	return true;
}