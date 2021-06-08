#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include "GLShader.h"
#include "TextureManager.h"
#include "Vertex.h"
#include "Skeleton.h"

//Maximum textures = 3

//TODO(nick): Holy shit, refactor this mess
class Mesh {
public:
	Mesh();
	~Mesh();
	void AddVertex(glm::vec3 position, glm::vec2 uv, glm::vec3 normal);
	void AddIndex(uint32_t i);
	void DefineBoneAndWeights(unsigned int vertexID, glm::ivec4 bones, glm::vec4 weights);

	uint32_t getEBO();
	uint32_t getVAO();

	VertexData* getVertexData();

	void GenerateMesh();
	void RenderMesh(GLShader& shader, bool useIndexing, bool forwardTextures, bool forceNoAnim);

	void AddTexture(std::string type, std::string path);

	void Translate(glm::vec3 position);
	void Rotate(float angle, glm::vec3 axis);
	void Scale(glm::vec3 modifier);
	void LookAt(glm::vec3 origin, glm::vec3 center, glm::vec3 up);

	void setSkeleton(Skeleton* skele);
	Skeleton* skeleton;

	glm::vec3 Min() { return mMin; }
	glm::vec3 Max() { return mMax; }
private:
	VertexData mVertices;

	bool isAnimated;

	std::vector<GLTexture*> mTextures;
	glm::vec3 mMin, mMax;
	glm::mat4 mModelMatrix;

	uint32_t mVAO, mVertBuff, mUVBuff, mNormalBuff, mBoneBuffer, mWeightBuffer, mEBO;
};