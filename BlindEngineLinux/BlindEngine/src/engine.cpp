#include "engine.h"

engine* engine::__instance = nullptr;


engine* engine::instance()
{
	if (__instance == nullptr)
		__instance = new engine();
	return __instance;
}

engine::engine()
{
}


engine::~engine()
{
}
