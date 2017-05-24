#include "Material.h"
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

MaterialMatte::MaterialMatte() : ambient(1.0f), diffuse(1.0f), specular(1.0f) {
	type = "matte";
}

void MaterialMatte::loadFile(const char* path) {
	std::ifstream imat(path);
	if (imat.fail()) {
		perror(path);
		return;
	}

	std::string rawLine, x, y, z;
	std::istringstream iss;
	float fx, fy, fz;

	//Ambient
	std::getline(imat, rawLine);
	iss = std::istringstream(rawLine);
	std::getline(iss, x, ' ');
	std::getline(iss, y, ' ');
	std::getline(iss, z, ' ');
	fx = std::stof(x);
	fy = std::stof(y);
	fz = std::stof(z);
	SetAmbient(fx, fy, fz);

	//Diffuse
	std::getline(imat, rawLine);
	iss = std::istringstream(rawLine);
	std::getline(iss, x, ' ');
	std::getline(iss, y, ' ');
	std::getline(iss, z, ' ');
	fx = std::stof(x);
	fy = std::stof(y);
	fz = std::stof(z);
	SetDiffuse(fx, fy, fz);

	//Specular
	std::getline(imat, rawLine);
	iss = std::istringstream(rawLine);
	std::getline(iss, x, ' ');
	std::getline(iss, y, ' ');
	std::getline(iss, z, ' ');
	fx = std::stof(x);
	fy = std::stof(y);
	fz = std::stof(z);
	SetSpecular(fx, fy, fz);

	std::getline(imat, rawLine);
	float shine = std::stof(rawLine);
	SetShine(shine);

	imat.close();
}

void MaterialMatte::SetAmbient(float x, float y, float z) {
	ambient = glm::vec3(x, y, z);
}

void MaterialMatte::SetDiffuse(float x, float y, float z) {
	diffuse = glm::vec3(x, y, z);
}

void MaterialMatte::SetSpecular(float x, float y, float z) {
	specular = glm::vec3(x, y, z);
}

void MaterialMatte::SetShine(float s) {
	this->shininess = s;
}

MaterialTexture::MaterialTexture() {
	type = "texture";
}

void MaterialTexture::loadFile(const char* path) {
	//Stubby stub
}