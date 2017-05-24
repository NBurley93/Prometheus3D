#pragma once
#include <GL\glew.h>
#include <string>
#include <vector>

class GLShader {
public:
	GLShader();
	~GLShader();
	void compile(const std::string& shaderFile);
	void link();

	void use();
	void unuse();
	GLuint getUniformLocation(const std::string& uniformName);
	static void enableDebugOutput();
	bool hasAttribute(const std::string& attributeName);
	bool hasUniform(const std::string& uniformName);

private:
	GLuint _programID, _vertexID, _fragmentID, _geometryID;
	unsigned int _numAttributes;

	static bool _debugOutput;
	bool _hasGeometry;

	void addAttribute(const std::string& attributeName);
	std::vector<std::string> _attributes;
};