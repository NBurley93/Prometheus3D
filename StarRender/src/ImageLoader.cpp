#include "ImageLoader.h"
#include <SOIL.h>

GLTexture* ImageLoader::loadTexture(const char* path) {
	GLTexture* t = new GLTexture();

	unsigned char* img;
	int channels;

	img = SOIL_load_image(path, &t->width, &t->height, &channels, SOIL_LOAD_AUTO);

	glGenTextures(1, &t->id);
	glBindTexture(GL_TEXTURE_2D, t->id);
	if (channels == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->width, t->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	}
	else {
		if (channels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t->width, t->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		}
	}

	//mipmaps
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(img);
	return t;
}