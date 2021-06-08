#include "GLTexture.h"
#include <glad/glad.h>

GLTexture::~GLTexture() {
    if (_id != 0) {
        glDeleteTextures(1, &_id);
    }
}

GLTexture::GLTexture(const uint32_t& id, int width, int height, const std::string& type) : _id(id), _width(width), _height(height), _type(type) {}
