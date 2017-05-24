#pragma once
#include <string>

class Globals {
public:
	std::string input_text;
	bool uLighting;
	bool uNormalMapping;
	bool uTexturing;
	bool uWireframe;
	bool uBBOX;
	bool uShadows;
	bool uDebugShadows;
	bool uAnimation;

	int shadowPCFSamples;

	static Globals* Get();
private:
	static Globals* mInst;
};