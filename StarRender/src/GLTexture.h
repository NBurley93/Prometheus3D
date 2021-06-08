#pragma once
#include <string>

class GLTexture {
	int _width, _height;
	uint32_t _id;
	std::string _type;

public:
	GLTexture(const uint32_t& id, int width, int height, const std::string& type);
	~GLTexture();

	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
	uint32_t GetID() const { return _id; }
	std::string GetType() const { return _type; }

	void SetType(const std::string& type) { _type = type; }
};