#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "GLShader.h"
#include <map>

struct FontCharacter {
	GLuint TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint Advance;
};

class Font {
public:
	void Init(std::string filePath, int pSize);
	void setColor(glm::vec3 nColor);
	void RenderText(GLShader& shdr, std::string text, GLfloat x, GLfloat y, GLfloat scale);
private:
	GLuint mVAO, mVBO;
	glm::vec3 mTextColor;
	std::map<GLchar, FontCharacter> mFontCharacters;
};