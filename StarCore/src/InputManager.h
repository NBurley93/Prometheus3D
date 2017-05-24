#pragma once
#include <unordered_map>

class InputManager {
public:
	void update();

	void pressKey(unsigned int keyID);
	void releaseKey(unsigned int keyID);
	bool isKeyDown(unsigned int keyID);
	bool isKeyPressed(unsigned int keyID);
	void setMouse(float x, float y);
	float mouse_x();
	float mouse_y();

private:
	bool wasKeyDown(unsigned int keyID);
	std::unordered_map<unsigned int, bool> _keyMap;
	std::unordered_map<unsigned int, bool> _prevKeyMap;
	float mX, mY;
};