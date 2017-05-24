#pragma once
#include "GLTexture.h"
#include <map>
#include <string>

class TextureManager {
public:
	GLTexture* getTexture(const std::string& filePath);
	void Clean();
	void InitializeFreetype();
	static TextureManager* Get();
private:
	GLTexture* find(const std::string& filePath);
	static TextureManager* _inst;
	std::map<std::string, GLTexture*> _textureMap;
};