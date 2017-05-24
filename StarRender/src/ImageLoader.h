#pragma once
#include <GL/glew.h>
#include <string>
#include "GLTexture.h"

class ImageLoader {
public:
	static GLTexture* loadTexture(const char* path);
};