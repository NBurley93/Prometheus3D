#pragma once
#include <string>
#include "Camera.h"

class Skybox {
public:
	Skybox();
	~Skybox();

	void loadCubemap(std::string path);
	void Init();
	void Render(Camera& cam);

	uint32_t getTex() { return mTextureID; }
private:
	uint32_t mTextureID, mVAO, mVBO;
};