#pragma once
#include <vector>
#include <glm/glm.hpp>

//TODO(nick): This is awful, fix this
struct VertexStruct {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	uint32_t index;

	unsigned int bone_ids[4];
	float weights[4];
};

struct VertexData {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<uint32_t> indices;
	std::vector<glm::ivec4> boneids;
	std::vector<glm::vec4> weights;
};