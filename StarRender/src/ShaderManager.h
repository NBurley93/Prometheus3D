#pragma once
#include "GLShader.h"
#include <map>
#include <string>

class ShaderManager {
public:
	GLShader* getShdr(const std::string& id);
	void Clean();
	static ShaderManager* Get();
private:
	GLShader* find(const std::string& filePath);
	GLShader* addShdr(const std::string& filePath);
	static ShaderManager* _inst;
	std::map<std::string, GLShader*> mShaderMap;
};