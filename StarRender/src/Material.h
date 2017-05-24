#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "GLShader.h"
#include "GLTexture.h"
#include <vector>

const int MAX_TEXTURES = 12;

struct Material {
	std::string type;
	virtual void loadFile(const char* path) = 0;

	Material() {};
	virtual ~Material() {}
};

struct MaterialMatte : public Material {
	MaterialMatte();

	void loadFile(const char* path);

	void SetAmbient(float x, float y, float z);
	void SetDiffuse(float x, float y, float z);
	void SetSpecular(float x, float y, float z);
	void SetShine(float s);

	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;
	float shininess;
};

struct MaterialTexture : public Material {
	MaterialTexture();

	void loadFile(const char* path);

	std::vector<GLTexture*> mDiffuseTextures;
	std::vector<GLTexture*> mSpecularTextures;
	std::vector<GLTexture*> mNormalTextures;
};