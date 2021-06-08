#pragma once
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
	uint32_t getUniformLocation(const std::string& uniformName);
	static void enableDebugOutput();
	bool hasAttribute(const std::string& attributeName);
	bool hasUniform(const std::string& uniformName);

private:
	uint32_t _programID, _vertexID, _fragmentID, _geometryID;
	unsigned int _numAttributes;

	static bool _debugOutput;
	bool _hasGeometry;

	void addAttribute(const std::string& attributeName);
	std::vector<std::string> _attributes;
};