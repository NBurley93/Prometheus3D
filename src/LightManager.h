#pragma once
#include <Light.h>
const int MAX_LIGHTS = 2;

class LightManager {
public:
	Light Lights[MAX_LIGHTS];
	Light globalLight;

	static LightManager* Get();
private:
	static LightManager* mInst;
};