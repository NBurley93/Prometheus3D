#pragma once
#include <GL/glew.h>
#include <SDL2/SDL.h>

class Window {
public:
	void CreateWindow(int Width, int Height, const char* title);
	void SwapBuffers();
	void DestroyWindow();
private:
	SDL_Window* mHandle;
	SDL_GLContext mContext;
};