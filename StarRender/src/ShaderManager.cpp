#include "ShaderManager.h"

ShaderManager* ShaderManager::_inst = nullptr;

ShaderManager* ShaderManager::Get() {
	if (_inst == nullptr) {
		_inst = new ShaderManager();
	}
	return _inst;
}

void ShaderManager::Clean() {
	for (auto mit = mShaderMap.begin(); mit != mShaderMap.end(); mit++) {
		delete mit->second;
	}
	mShaderMap.clear();
}

GLShader* ShaderManager::getShdr(const std::string& id) {
	//Get true path first
	std::string tPath = "./data/shaders/" + id + ".shader";

	GLShader* r = find(tPath);
	if (r == nullptr) {
		//Gotta load
		r = addShdr(tPath);
	}
	return r; //Return cached or loaded texture
}

GLShader* ShaderManager::addShdr(const std::string& filePath) {
	GLShader* n = new GLShader();
	n->compile(filePath);
	n->link();
	mShaderMap[filePath] = n;
	return n;
}

GLShader* ShaderManager::find(const std::string& filePath) {
	auto mit = mShaderMap.find(filePath);
	if (mit != mShaderMap.end()) {
		return mit->second;
	}
	return nullptr;
}