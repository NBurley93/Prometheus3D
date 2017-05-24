#include "Window.h"
#include <iostream>

void Window::CreateWindow(int Width, int Height, const char* title) {
	mHandle = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, SDL_WINDOW_OPENGL);
	if (mHandle == NULL) {
		return;
	}

	mContext = SDL_GL_CreateContext(mHandle);
	SDL_GL_MakeCurrent(mHandle, mContext);

	GLenum glewErr = glewInit();
	if (glewErr != GLEW_OK) {
		std::printf("%s\n", glewGetErrorString(glewErr));
		return;
	}
}

void Window::SwapBuffers() {
	SDL_GL_SwapWindow(mHandle);
}

void Window::DestroyWindow() {
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mHandle);
}