#pragma once
#include <GL/glew.h>
#include <string>

struct GLTexture {
	~GLTexture() {
		if (id != 0) {
			glDeleteTextures(1, &id);
		}
	}
	int width, height;
	GLuint id;
	std::string type;
};