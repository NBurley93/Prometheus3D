#pragma once
//Loads a .shader into memory
#include <string>
#include <fstream>
#include <sstream>

class ShaderIO {
public:
	static void LoadFile(const std::string& filePath, std::string& attributeData, std::string& vertexData, std::string& fragmentData, std::string& geometryData);
};