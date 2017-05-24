#include "InputManager.h"
#include <iostream>

void InputManager::update() {
	for (auto mit = _keyMap.begin(); mit != _keyMap.end(); mit++) {
		_prevKeyMap[mit->first] = mit->second;
	}
}

void InputManager::pressKey(unsigned int keyID) {
	_keyMap[keyID] = true;
}

void InputManager::releaseKey(unsigned int keyID) {
	_keyMap[keyID] = false;
}

bool InputManager::isKeyDown(unsigned int keyID) {
	auto mit = _keyMap.find(keyID);
	if (mit != _keyMap.end()) {
		return mit->second;
	}
	return false;
}

bool InputManager::wasKeyDown(unsigned int keyID) {
	auto mit = _prevKeyMap.find(keyID);
	if (mit != _prevKeyMap.end()) {
		return mit->second;
	}
	return false;
}

bool InputManager::isKeyPressed(unsigned int keyID) {
	if (isKeyDown(keyID) == true && wasKeyDown(keyID) == false) {
		return true;
	}
	return false;
}

void InputManager::setMouse(float x, float y) {
	mX = x;
	mY = y;
}

float InputManager::mouse_x() {
	return mX;
}

float InputManager::mouse_y() {
	return mY;
}