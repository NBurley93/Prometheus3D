#include "Light.h"

Light::Light() {
	ambient = glm::vec3(1.0f);
	diffuse = glm::vec3(1.0f);
	specular = glm::vec3(1.0f);
	direction = glm::vec4(0.0f);
	enabled = false;
}

void Light::toShader(GLShader& shdr, int index) {
	//Forwards the material to the shader uniforms
	glUniform3f(shdr.getUniformLocation("lights[" + std::to_string(index) + "].ambient"), ambient.x, ambient.y, ambient.z);
	glUniform3f(shdr.getUniformLocation("lights[" + std::to_string(index) + "].diffuse"), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(shdr.getUniformLocation("lights[" + std::to_string(index) + "].specular"), specular.x, specular.y, specular.z);
	glUniform4f(shdr.getUniformLocation("lights[" + std::to_string(index) + "].direction"), direction.x, direction.y, direction.z, direction.w);
	glUniform1i(shdr.getUniformLocation("lights[" + std::to_string(index) + "].enabled"), enabled);
}

void Light::toShader(GLShader& shdr) {
	glUniform3f(shdr.getUniformLocation("light.ambient"), ambient.x, ambient.y, ambient.z);
	glUniform3f(shdr.getUniformLocation("light.diffuse"), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(shdr.getUniformLocation("light.specular"), specular.x, specular.y, specular.z);
	glUniform4f(shdr.getUniformLocation("light.direction"), direction.x, direction.y, direction.z, direction.w);
	glUniform1i(shdr.getUniformLocation("light.enabled"), enabled);
}