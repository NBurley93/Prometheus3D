#pragma once
#include <glm/glm.hpp>
#include "GLShader.h"

struct Light {
	Light();

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec4 direction;
	bool enabled;

	void toShader(GLShader& shdr, int index);
	void toShader(GLShader& shdr);
};