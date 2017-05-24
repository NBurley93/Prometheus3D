#pragma once
#include <glm/glm.hpp>

struct Particle {
	Particle();

	glm::vec3 position, velocity;
	glm::vec4 color;
	float life, rotation, constLife;
};