#pragma once
//Emit particles, manage them, and draw them in an instanced fasion
#include <GL/glew.h>
#include "Particle.h"
#include <vector>
#include "TextureManager.h"
#include "Camera.h"
#include "Mesh.h"

const int MAX_PARTICLES = 1000;

class ParticleEmitter {
public:
	~ParticleEmitter();
	void Init(float X, float Y, float Z, std::string texturePath);
	void Emit(int index, float dx, float dy, float dz, float life);
	void Update(float dt);
	void RenderAll(const Camera& camera);
private:
	Mesh* mMesh;
	std::vector<Particle> mParticles;
	float x, y, z;
	GLTexture* mPTexture;
};