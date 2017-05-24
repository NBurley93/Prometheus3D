#include "Sprite2D.h"
#include "ShaderManager.h"
#include <glm/gtc/type_ptr.hpp>

Sprite2D::Sprite2D() : mVAO(0), mVertBuffer(0), mModelMatrix(1.0f) {}

Sprite2D::~Sprite2D() {
	if (mVertBuffer != 0) {
		glDeleteBuffers(1, &mVertBuffer);
		glDeleteBuffers(1, &mColorBuffer);
		glDeleteBuffers(1, &mUVBuffer);
	}

	if (mVAO != 0) {
		glDeleteVertexArrays(1, &mVAO);
	}
}

void Sprite2D::Init(GLTexture* texture) {
	if (mVertBuffer == 0) {
		glGenBuffers(1, &mVertBuffer);
		glGenBuffers(1, &mColorBuffer);
		glGenBuffers(1, &mUVBuffer);
	}
	if (mVAO == 0) {
		glGenVertexArrays(1, &mVAO);
	}

	mTexture = texture;

	mVertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	mUVS.push_back(glm::vec2(0.0f, 0.0f));

	mVertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	mUVS.push_back(glm::vec2(1.0f, 0.0f));

	mVertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	mUVS.push_back(glm::vec2(1.0f, 1.0f));

	mVertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	mUVS.push_back(glm::vec2(1.0f, 1.0f));

	mVertices.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	mUVS.push_back(glm::vec2(0.0f, 1.0f));

	mVertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	mUVS.push_back(glm::vec2(0.0f, 0.0f));

	for (int i = 0; i < 6; i++) {
		mColors.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVertBuffer);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), mVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec4), mColors.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mUVBuffer);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec2), mUVS.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, mVertBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mUVBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);
}

void Sprite2D::Init(std::string texture) {
	if (mVertBuffer == 0) {
		glGenBuffers(1, &mVertBuffer);
		glGenBuffers(1, &mColorBuffer);
		glGenBuffers(1, &mUVBuffer);
	}
	if (mVAO == 0) {
		glGenVertexArrays(1, &mVAO);
	}

	mTexture = TextureManager::Get()->getTexture(texture);

	mVertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	mUVS.push_back(glm::vec2(0.0f, 0.0f));

	mVertices.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	mUVS.push_back(glm::vec2(1.0f, 0.0f));

	mVertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	mUVS.push_back(glm::vec2(1.0f, 1.0f));

	mVertices.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
	mUVS.push_back(glm::vec2(1.0f, 1.0f));

	mVertices.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	mUVS.push_back(glm::vec2(0.0f, 1.0f));

	mVertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	mUVS.push_back(glm::vec2(0.0f, 0.0f));

	for (int i = 0; i < 6; i++) {
		mColors.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVertBuffer);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), mVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec4), mColors.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mUVBuffer);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec2), mUVS.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, mVertBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mColorBuffer);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mUVBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);
}
void Sprite2D::Translate(glm::vec3 position) {
	mModelMatrix = glm::translate(mModelMatrix, position);
}

void Sprite2D::Rotate(float angle, glm::vec3 axis) {
	mModelMatrix = glm::rotate(mModelMatrix, angle, axis);
}

void Sprite2D::Scale(glm::vec3 modifier) {
	mModelMatrix = glm::scale(mModelMatrix, modifier);
}

void Sprite2D::SetModelMatrix(glm::mat4 mat) {
	mModelMatrix = mat;
}

void Sprite2D::SetAlpha(float a) {
	mAlpha = a;
}

void Sprite2D::Draw(Camera& camera, float x, float y, float w, float h) {
	glBindVertexArray(mVAO);

	Translate(glm::vec3(0.0f, h, 0.0f));
	Translate(glm::vec3(x, y, 0.0f));
	Scale(glm::vec3(w, -h, 1.0f));

	ShaderManager::Get()->getShdr("sprite_2d")->use();
	glUniform1i(ShaderManager::Get()->getShdr("sprite_2d")->getUniformLocation("sampler"), 0);
	glUniform1f(ShaderManager::Get()->getShdr("sprite_2d")->getUniformLocation("alpha"), mAlpha);
	glUniformMatrix4fv(ShaderManager::Get()->getShdr("sprite_2d")->getUniformLocation("projection"), 1, false, glm::value_ptr(camera.getMatrix("ortho")));
	glUniformMatrix4fv(ShaderManager::Get()->getShdr("sprite_2d")->getUniformLocation("model"), 1, false, glm::value_ptr(mModelMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTexture->id);

	glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

	glBindVertexArray(0);
	mModelMatrix = glm::mat4(1.0f);
	ShaderManager::Get()->getShdr("sprite_2d")->unuse();
}