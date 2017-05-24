#include "BinHelper.h"

std::string BinHelper::readBin(std::ifstream& stream, unsigned int size) {
	std::string buff = "";
	char* charbuffer = new char[size];
	stream.read(charbuffer, size);
	buff.append(charbuffer, size);
	delete charbuffer;
	return buff;
}

unsigned int BinHelper::bToInt(std::string buffer) {
	unsigned char a[4];
	a[0] = buffer[0];
	a[1] = buffer[1];
	a[2] = buffer[2];
	a[3] = buffer[3];

	unsigned int num = *(int*)a;
	return num;
}

void BinHelper::writeInt(unsigned int i, std::ofstream& stream) {
	stream.write(reinterpret_cast<const char*>(&i), sizeof(i));
}

void BinHelper::writeString(std::string str, std::ofstream& stream) {
	stream.write(str.c_str(), str.length());
}