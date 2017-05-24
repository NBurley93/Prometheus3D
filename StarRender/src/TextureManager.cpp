#include "TextureManager.h"
#include "ImageLoader.h"

TextureManager* TextureManager::_inst = nullptr;

TextureManager* TextureManager::Get() {
	if (_inst == nullptr) {
		_inst = new TextureManager();
	}
	return _inst;
}

void TextureManager::Clean() {
	for (auto mit = _textureMap.begin(); mit != _textureMap.end(); mit++) {
		delete mit->second;
	}
	_textureMap.clear();
}

GLTexture* TextureManager::getTexture(const std::string& filePath) {
	GLTexture* rTex = find(filePath);
	if (rTex == nullptr) {
		//Gotta load
		rTex = ImageLoader::loadTexture(filePath.c_str());
		_textureMap[filePath] = rTex; //Cache it
	}
	return rTex; //Return cached or loaded texture
}

GLTexture* TextureManager::find(const std::string& filePath) {
	auto mit = _textureMap.find(filePath);
	if (mit != _textureMap.end()) {
		return mit->second;
	}
	return nullptr;
}