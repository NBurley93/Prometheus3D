#include "ShaderIO.h"
#include "BinHelper.h"

/*
Shader file format:

Header:
	char[4] SHDR (file-identifier)
	unsigned int attributeSize
	unsigned int vertexSize
	unsigned int fragmentSize
	unsigned int geometrySize

Body:
	char[attributeSize] attributeData
	char[vertexSize] vertexData
	char[fragmentSize] fragmentData
	char[geometrySize] geometryData
*/

using namespace BinHelper;

void ShaderIO::LoadFile(const std::string& filePath, std::string& attributeData, std::string& vertexData, std::string& fragmentData, std::string& geometryData) {
	//Load shader at filePath and load it's respective parts into attributeData, vertexData, and fragmentData
	std::ifstream lFile(filePath, std::ios::binary);
	if (lFile.fail()) {
		perror(filePath.c_str());
		attributeData = "NULL";
		vertexData = "NULL";
		fragmentData = "NULL";
		geometryData = "NULL";
		return;
	}

	unsigned int attrSize, vertSize, fragSize, geoSize, fileSize;
	std::string fileID;

	fileID = readBin(lFile, 4);
	

	if (fileID == "SHDR") {
		//Valid file
		attrSize = bToInt(readBin(lFile, sizeof(unsigned int)));
		vertSize = bToInt(readBin(lFile, sizeof(unsigned int)));
		fragSize = bToInt(readBin(lFile, sizeof(unsigned int)));
		geoSize = bToInt(readBin(lFile, sizeof(unsigned int)));
		attributeData = readBin(lFile, attrSize);
		vertexData = readBin(lFile, vertSize);
		fragmentData = readBin(lFile, fragSize);
		if (geoSize > 0) {
			geometryData = readBin(lFile, geoSize);
		}
		else {
			geometryData = "NULL";
		}

		fileSize = 4 + (sizeof(unsigned int) * 4) + attrSize + vertSize + fragSize + geoSize;
		printf("%s - %d bytes\n", filePath.c_str(), fileSize);
	}
	else {
		printf("%s is an invalid or corrupt shader file\n", filePath.c_str());
		attributeData = "NULL";
		vertexData = "NULL";
		fragmentData = "NULL";
		geometryData = "NULL";
	}
	lFile.close();
}