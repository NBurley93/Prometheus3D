#pragma once
#include <string>
#include "GLTexture.h"

class ImageLoader {
public:
	static GLTexture* loadTexture(const char* path);
};