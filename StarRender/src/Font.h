#pragma once
#include <glm/glm.hpp>
#include "GLShader.h"
#include <map>

struct FontCharacter {
	uint32_t TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	uint32_t Advance;
};

class Font {
public:
	void Init(std::string filePath, int pSize);
	void setColor(glm::vec3 nColor);
	void RenderText(GLShader& shdr, std::string text, float x, float y, float scale);
private:
	uint32_t mVAO, mVBO;
	glm::vec3 mTextColor;
	std::map<uint8_t, FontCharacter> mFontCharacters;
};