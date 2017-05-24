#include "Skybox.h"
#include <SOIL.h>
#include <vector>
#include "ShaderManager.h"
#include <glm/gtc/type_ptr.hpp>

Skybox::Skybox() {
	if (mTextureID == 0) {
		glGenTextures(1, &mTextureID);
	}
}

Skybox::~Skybox() {
	if (mTextureID != 0) {
		glDeleteTextures(1, &mTextureID);
	}
	if (mVAO != 0) {
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mVBO);
	}
}

void Skybox::loadCubemap(std::string path) {
	std::vector<std::string> faces;

	faces.push_back(path + "/front.tga");
	faces.push_back(path + "/back.tga");
	faces.push_back(path + "/top.tga");
	faces.push_back(path + "/bottom.tga");
	faces.push_back(path + "/right.tga");
	faces.push_back(path + "/left.tga");

	int width, height, channels;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);
	for (GLuint i = 0; i < faces.size(); i++) {
		
		image = SOIL_load_image(faces[i].c_str(), &width, &height, &channels, SOIL_LOAD_RGB);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Skybox::Init() {
	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	// Setup skybox VAO
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
}

void Skybox::Render(Camera& cam) {
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	ShaderManager::Get()->getShdr("skybox")->use();
	glUniformMatrix4fv(ShaderManager::Get()->getShdr("skybox")->getUniformLocation("view"), 1, false, glm::value_ptr(glm::mat4(glm::mat3(cam.getMatrix("view")))));
	glUniformMatrix4fv(ShaderManager::Get()->getShdr("skybox")->getUniformLocation("projection"), 1, false, glm::value_ptr(cam.getMatrix("projection")));

	glBindVertexArray(mVAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(ShaderManager::Get()->getShdr("skybox")->getUniformLocation("skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, getTex());

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glEnable(GL_CULL_FACE);
	glBindVertexArray(0);
	ShaderManager::Get()->getShdr("skybox")->unuse();
	glDepthMask(GL_TRUE); //Make sure that depth mask is reenabled
}