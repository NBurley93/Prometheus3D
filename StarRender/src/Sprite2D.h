#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GLShader.h"
#include "Camera.h"
#include "TextureManager.h"

class Sprite2D {
public:
	Sprite2D();
	~Sprite2D();

	void Init(std::string texture);
	void Init(GLTexture* texture);
	void Draw(Camera& camera, float x, float y, float w, float h);

	void SetAlpha(float a);

	void Translate(glm::vec3 position);
	void Rotate(float angle, glm::vec3 axis);
	void Scale(glm::vec3 modifier);
	void SetModelMatrix(glm::mat4 mat);
private:
	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec4> mColors;
	std::vector<glm::vec2> mUVS;

	float mAlpha;

	glm::mat4 mModelMatrix;

	GLTexture* mTexture;

	GLuint mVAO, mVertBuffer, mColorBuffer, mUVBuffer;
};