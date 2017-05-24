#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

struct VertexStruct {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	GLuint index;

	unsigned int bone_ids[4];
	float weights[4];
};

struct VertexData {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<GLuint> indices;
	std::vector<glm::ivec4> boneids;
	std::vector<glm::vec4> weights;
};