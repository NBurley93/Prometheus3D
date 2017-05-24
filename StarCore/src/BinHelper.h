#pragma once
#include <string>
#include <fstream>

namespace BinHelper {
	//Reading
	std::string readBin(std::ifstream& stream, unsigned int size);
	unsigned int bToInt(std::string buffer);

	//Writing
	void writeInt(unsigned int i, std::ofstream& stream);
	void writeString(std::string str, std::ofstream& stream);
};