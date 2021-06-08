#include "GLShader.h"
#include <ShaderIO.h>
#include <sstream>
#include <glad/glad.h>

bool GLShader::_debugOutput = false;
void GLShader::enableDebugOutput() {
	_debugOutput = true;
}

GLShader::GLShader() : _programID(0), _vertexID(0), _fragmentID(0), _numAttributes(0), _geometryID(0), _hasGeometry(false) {}

GLShader::~GLShader() {}

void GLShader::compile(const std::string& shaderFile) {
	std::string attributeDat, vertexDat, fragmentDat, geometryDat;
	ShaderIO::LoadFile(shaderFile, attributeDat, vertexDat, fragmentDat, geometryDat);
	if (attributeDat != "NULL" && vertexDat != "NULL" && fragmentDat != "NULL") {
		//Load successful
		const char* vertexPtr = vertexDat.c_str();
		const char* fragPtr = fragmentDat.c_str();
		const char* geoPtr = geometryDat.c_str();
		if (geometryDat != "NULL") {
			_hasGeometry = true;
		}

		_vertexID = glCreateShader(GL_VERTEX_SHADER);
		_fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
		_programID = glCreateProgram();

		if (_hasGeometry) {
			_geometryID = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(_geometryID, 1, &geoPtr, nullptr);
		}

		glShaderSource(_vertexID, 1, &vertexPtr, nullptr);
		glShaderSource(_fragmentID, 1, &fragPtr, nullptr);

		//Compile vertex
		glCompileShader(_vertexID);
		GLint success = 0;
		glGetShaderiv(_vertexID, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(_vertexID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> errLog(maxLength);
			glGetShaderInfoLog(_vertexID, maxLength, &maxLength, &errLog[0]);

			glDeleteShader(_vertexID);

			printf("[Shader_Compile] %s\n", &errLog[0]);
			printf("Failed to compile shader!\n");
			exit(-1);
		}
		if (_debugOutput)
			printf("[Shader] Vertex compiled within %s\n", shaderFile.c_str());

		//Compile geometry
		if (_hasGeometry) {
			glCompileShader(_geometryID);
			success = 0;
			glGetShaderiv(_geometryID, GL_COMPILE_STATUS, &success);
			if (success == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(_geometryID, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<char> errLog(maxLength);
				glGetShaderInfoLog(_geometryID, maxLength, &maxLength, &errLog[0]);

				glDeleteShader(_geometryID);

				printf("[Shader_Compile] %s\n", &errLog[0]);
				printf("Failed to compile shader!\n");
				exit(-1);
			}
			if (_debugOutput)
				printf("[Shader] Geometry compiled within %s\n", shaderFile.c_str());
		}

		//Compile fragment
		glCompileShader(_fragmentID);
		success = 0;
		glGetShaderiv(_fragmentID, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(_fragmentID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> errLog(maxLength);
			glGetShaderInfoLog(_fragmentID, maxLength, &maxLength, &errLog[0]);

			glDeleteShader(_fragmentID);

			printf("[Shader_Compile] %s\n", &errLog[0]);
			printf("Failed to compile shader!\n");
			exit(-1);
		}
		if (_debugOutput)
			printf("[Shader] Fragment compiled within %s\n", shaderFile.c_str());

		//Now to load attributes in
		std::stringstream sstr;
		std::string line;
		sstr << attributeDat;
		while (std::getline(sstr, line, '\n')) {
			addAttribute(line);
		}
		if (_debugOutput) {
			printf("[Shader] Attributes parsed within %s\n", shaderFile.c_str());
			printf("[Shader] Compiled %s\n", shaderFile.c_str());
		}
	}
	else {
		printf("Failed to compile shader!\n");
		exit(-1);
	}
}

void GLShader::link() {
	glAttachShader(_programID, _vertexID);
	glAttachShader(_programID, _fragmentID);
	if (_hasGeometry) {
		glAttachShader(_programID, _geometryID);
	}
	glLinkProgram(_programID);

	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<char> infoLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(_programID);
		glDeleteShader(_vertexID);
		glDeleteShader(_fragmentID);

		printf("Failed to link shader\n");
		exit(-1);
	}

	glDetachShader(_programID, _vertexID);
	glDetachShader(_programID, _fragmentID);
	if (_debugOutput)
		printf("[Shader] Shader linked\n");

	//Print uniforms
	if (_debugOutput) {
		printf("[Shader] Uniforms\n");
		GLint count;
		const GLsizei bufSize = 48;
		GLchar name[bufSize];
		GLsizei length = 0;
		GLenum type = 0;
		GLint size = 0;
		glGetProgramiv(_programID, GL_ACTIVE_UNIFORMS, &count);
		for (int i = 0; i < count; i++) {
			glGetActiveUniform(_programID, (GLuint)i, bufSize, &length, &size, &type, name);

			std::string typeName = "";
			switch (type) {
			case GL_SAMPLER_2D:
				typeName = "sampler2D";
				break;

			case GL_SAMPLER_CUBE:
				typeName = "samplerCube";
				break;

			case GL_FLOAT_VEC3:
				typeName = "vec3";
				break;

			case GL_FLOAT_VEC4:
				typeName = "vec4";
				break;

			case GL_BOOL:
				typeName = "bool";
				break;

			case GL_FLOAT_MAT4:
				typeName = "mat4";
				break;

			default:
				typeName = "Unknown";
				break;
			}

			std::printf("[Shader] Uniform #%d %s %s\n", i, typeName.c_str(), name);
		}
		std::printf("\n");
	}
}

void GLShader::addAttribute(const std::string& attributeName) {
	_attributes.push_back(attributeName);
	glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
}

void GLShader::use() {
	glUseProgram(_programID);
	for (int i = 0; i < _numAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}

void GLShader::unuse() {
	glUseProgram(0);
	for (int i = 0; i < _numAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}

uint32_t GLShader::getUniformLocation(const std::string& uniformName) {
	GLuint loc = glGetUniformLocation(_programID, uniformName.c_str());
	if (loc == GL_INVALID_INDEX) {
		printf("[Shader] Uniform %s does not exist in shader\n", uniformName.c_str());
	}
	return static_cast<uint32_t>(loc);
}

bool GLShader::hasUniform(const std::string& uniformName) {
	GLuint loc = glGetUniformLocation(_programID, uniformName.c_str());
	if (loc == GL_INVALID_INDEX) {
		return false;
	}
	return true;
}

bool GLShader::hasAttribute(const std::string& attributeName) {
	for (int i = 0; i < _attributes.size(); i++) {
		if (_attributes[i] == attributeName) {
			return true;
		}
	}
	return false;
}