#include "ImageLoader.h"
#include <SOIL/SOIL.h>
#include <glad/glad.h>

GLTexture* ImageLoader::loadTexture(const char* path) {
	unsigned char* img;
	int channels;

	int width, height;

	img = SOIL_load_image(path, &width, &height, &channels, SOIL_LOAD_AUTO);

	uint32_t texId = 0;

	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	GLenum texFormat = GL_RGBA;

	if (channels == 3) {
		texFormat = GL_RGB;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, texFormat, width, height, 0, texFormat, GL_UNSIGNED_BYTE, img);

	//mipmaps
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(img);
	return new GLTexture(texId, width, height, "");
}