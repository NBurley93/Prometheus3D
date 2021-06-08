#pragma once
#include <SDL.h>

class Window {
	SDL_Window* mHandle;
	SDL_GLContext mContext;
	int _width, _height;

public:
	void CreateWindow(int Width, int Height, const char* title, bool Fullscreen = false);
	void SwapBuffers();
	void DestroyWindow();

	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
};