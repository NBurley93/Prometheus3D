#include "Mesh.h"
#include <glm/gtc/type_ptr.hpp>
#include "TextureManager.h"
#include "StarMath.h"
#include "ShaderManager.h"
#include <fstream>
#include <sstream>
#include "GL_Helper.h"

Mesh::Mesh() : mVAO(0), mVertBuff(0), mEBO(0), mModelMatrix(1.0f), isAnimated(false) {}

Mesh::~Mesh() {
	if (mVertBuff != 0) {
		glDeleteBuffers(1, &mVertBuff);
		glDeleteBuffers(1, &mUVBuff);
		glDeleteBuffers(1, &mNormalBuff);
		glDeleteBuffers(1, &mBoneBuffer);
		glDeleteBuffers(1, &mWeightBuffer);
		glDeleteBuffers(1, &mEBO);
	}

	if (mVAO != 0) {
		glDeleteVertexArrays(1, &mVAO);
	}
}

void Mesh::AddIndex(GLuint i) {
	mVertices.indices.push_back(i);
}

void Mesh::AddVertex(glm::vec3 position, glm::vec2 uv, glm::vec3 normal) {
	//Check minmax

	//Max
	if (position.x > mMax.x) {
		mMax.x = position.x;
	}
	if (position.y > mMax.y) {
		mMax.y = position.y;
	}
	if (position.z > mMax.z) {
		mMax.z = position.z;
	}

	//Min
	if (position.x < mMin.x) {
		mMin.x = position.x;
	}
	if (position.y < mMin.y) {
		mMin.y = position.y;
	}
	if (position.z < mMin.z) {
		mMin.z = position.z;
	}

	mVertices.vertices.push_back(position);
	mVertices.uvs.push_back(uv);
	mVertices.normals.push_back(normal);
}

void Mesh::DefineBoneAndWeights(unsigned int vertexID, glm::ivec4 bones, glm::vec4 weights) {
	mVertices.boneids[vertexID] = bones;
	mVertices.weights[vertexID] = weights;
	isAnimated = true;
}

VertexData* Mesh::getVertexData() {
	return &mVertices;
}

void Mesh::AddTexture(std::string type, std::string path) {
	GLTexture* t = TextureManager::Get()->getTexture(path);
	if (t != nullptr) {
		t->type = type;
		mTextures.push_back(t);
	}
}

void Mesh::GenerateMesh() {
	if (mVertBuff == 0) {
		glGenBuffers(1, &mVertBuff);
		glCheckError();
		glGenBuffers(1, &mUVBuff);
		glCheckError();
		glGenBuffers(1, &mNormalBuff);
		glCheckError();
		glGenBuffers(1, &mBoneBuffer);
		glCheckError();
		glGenBuffers(1, &mWeightBuffer);
		glCheckError();
		glGenBuffers(1, &mEBO);
		glCheckError();
	}
	if (mVAO == 0) {
		glGenVertexArrays(1, &mVAO);
		glCheckError();
	}

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVertBuff);
	glBufferData(GL_ARRAY_BUFFER, mVertices.vertices.size() * sizeof(glm::vec3), mVertices.vertices.data(), GL_STATIC_DRAW);
	glCheckError();

	glBindBuffer(GL_ARRAY_BUFFER, mUVBuff);
	glBufferData(GL_ARRAY_BUFFER, mVertices.uvs.size() * sizeof(glm::vec2), mVertices.uvs.data(), GL_STATIC_DRAW);
	glCheckError();

	glBindBuffer(GL_ARRAY_BUFFER, mNormalBuff);
	glBufferData(GL_ARRAY_BUFFER, mVertices.normals.size() * sizeof(glm::vec3), mVertices.normals.data(), GL_STATIC_DRAW);
	glCheckError();

	glBindBuffer(GL_ARRAY_BUFFER, mBoneBuffer);
	glBufferData(GL_ARRAY_BUFFER, mVertices.boneids.size() * sizeof(glm::vec3), mVertices.boneids.data(), GL_STATIC_DRAW);
	glCheckError();

	glBindBuffer(GL_ARRAY_BUFFER, mWeightBuffer);
	glBufferData(GL_ARRAY_BUFFER, mVertices.weights.size() * sizeof(glm::vec3), mVertices.weights.data(), GL_STATIC_DRAW);
	glCheckError();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mVertices.indices.size() * sizeof(GLuint), mVertices.indices.data(), GL_STATIC_DRAW);
	glCheckError();

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	glBindBuffer(GL_ARRAY_BUFFER, mVertBuff);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glCheckError();

	glBindBuffer(GL_ARRAY_BUFFER, mUVBuff);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glCheckError();

	glBindBuffer(GL_ARRAY_BUFFER, mNormalBuff);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glCheckError();

	glBindBuffer(GL_ARRAY_BUFFER, mBoneBuffer);
	glVertexAttribIPointer(3, 4, GL_INT, 0, 0);
	glCheckError();

	glBindBuffer(GL_ARRAY_BUFFER, mWeightBuffer);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_TRUE, 0, (void*)0);
	glCheckError();

	glBindVertexArray(0);
}

void Mesh::RenderMesh(GLShader& shader, bool useIndexing, bool forwardTextures, bool forceNoAnim) {
	if (isAnimated && !forceNoAnim) {
		skeleton->Update(1/ 60.0f);
	}

	glBindVertexArray(mVAO);

	if (forwardTextures) {
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		GLuint normalNr = 1;
		for (GLuint i = 0; i < mTextures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glCheckError();

			std::stringstream ss;
			std::string number;
			std::string name = mTextures[i]->type;
			if (name == "diffuse") {
				ss << diffuseNr++;
			}
			else {
				if (name == "specular") {
					ss << specularNr++;
				}
			}
			number = ss.str();

			glUniform1i(shader.getUniformLocation("material." + name + number), i);
			glCheckError();
			glBindTexture(GL_TEXTURE_2D, mTextures[i]->id);
			glCheckError();
		}
		glActiveTexture(GL_TEXTURE0);
	}

	if (isAnimated && !forceNoAnim) {
		glUniformMatrix4fv(shader.getUniformLocation("gBones"), skeleton->boneMats.size(), GL_FALSE, glm::value_ptr(skeleton->boneMats[0]));
	}

	glUniformMatrix4fv(shader.getUniformLocation("model"), 1, false, glm::value_ptr(mModelMatrix));
	glCheckError();

	if (useIndexing) {
		glDrawElements(GL_TRIANGLES, mVertices.indices.size(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, mVertices.vertices.size());
	}

	glBindVertexArray(0);

	mModelMatrix = glm::mat4(1.0f);
}

void Mesh::Translate(glm::vec3 position) {
	mModelMatrix = glm::translate(mModelMatrix, position);
}

void Mesh::Rotate(float angle, glm::vec3 axis) {
	mModelMatrix = glm::rotate(mModelMatrix, angle, axis);
}

void Mesh::Scale(glm::vec3 modifier) {
	mModelMatrix = glm::scale(mModelMatrix, modifier);
}

void Mesh::LookAt(glm::vec3 origin, glm::vec3 center, glm::vec3 up) {
	glm::mat4 rot = glm::lookAt(origin, center, up);
	mModelMatrix = mModelMatrix * rot;
}

GLuint Mesh::getEBO() {
	return mEBO;
}

GLuint Mesh::getVAO() {
	return mVAO;
}

void Mesh::setSkeleton(Skeleton* skele) {
	skeleton = skele;
}
