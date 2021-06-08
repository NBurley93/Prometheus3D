#include "Window.h"
#include <iostream>
#include <glad/glad.h>

void Window::CreateWindow(int Width, int Height, const char* title, bool Fullscreen) {
	_width = Width;
	_height = Height;

	uint32_t wFlags = SDL_WINDOW_OPENGL;
	if (Fullscreen) {
		wFlags |= SDL_WINDOW_FULLSCREEN;
	}

	mHandle = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Width, Height, wFlags);
	if (mHandle == NULL) {
		return;
	}

	mContext = SDL_GL_CreateContext(mHandle);
	SDL_GL_MakeCurrent(mHandle, mContext);

	// GLenum glewErr = glewInit();
	// if (glewErr != GLEW_OK) {
	// 	std::printf("%s\n", glewGetErrorString(glewErr));
	// 	return;
	// }
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::printf("Failed to initialize OpenGL context\n");
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