#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>

GLenum glCheckError_(const char* file, int line) {
	//OGL errors
	GLenum errCode;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		std::string error;
		switch (errCode) {
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << std::endl;
	}
	return errCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)