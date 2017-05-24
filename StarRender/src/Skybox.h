#pragma once
#include <GL/glew.h>
#include <string>
#include "Camera.h"

class Skybox {
public:
	Skybox();
	~Skybox();

	void loadCubemap(std::string path);
	void Init();
	void Render(Camera& cam);

	GLuint getTex() { return mTextureID; }
private:
	GLuint mTextureID, mVAO, mVBO;
};