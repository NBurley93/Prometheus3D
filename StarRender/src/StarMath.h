#pragma once
#include <glm/glm.hpp>
#include <vector>

void computeTangentBasis(std::vector<glm::vec3>& vertices, std::vector<glm::vec2> & uvs, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents) {
	for (int i = 0; i < vertices.size(); i += 3) {
		glm::vec3& v0 = vertices[i + 0];
		glm::vec3& v1 = vertices[i + 1];
		glm::vec3& v2 = vertices[i + 2];

		glm::vec2& uv0 = uvs[i + 0];
		glm::vec2& uv1 = uvs[i + 1];
		glm::vec2& uv2 = uvs[i + 2];

		glm::vec3 delta1 = v1 - v0;
		glm::vec3 delta2 = v2 - v0;

		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (delta1 * deltaUV2.y - delta2 * deltaUV1.y) * r;
		glm::vec3 bitangent = (delta2 * deltaUV1.x - delta1 * deltaUV2.x) * r;

		tangent = glm::normalize(tangent);
		bitangent = glm::normalize(bitangent);

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
	}
}