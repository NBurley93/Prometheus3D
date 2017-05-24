#include "ParticleEmitter.h"
#include "ShaderManager.h"

void ParticleEmitter::Init(float X, float Y, float Z, std::string texturePath) {
	for (int i = 0; i < MAX_PARTICLES; i++) {
		mParticles.push_back(Particle());
	}
	x = X;
	y = Y;
	z = Z;
	mPTexture = TextureManager::Get()->getTexture(texturePath);

	mMesh = new Mesh();
	mMesh->AddVertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mMesh->AddVertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mMesh->AddVertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mMesh->AddVertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mMesh->AddVertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mMesh->AddVertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	mMesh->GenerateMesh();
}

void ParticleEmitter::Update(float dt) {
	for (int i = 0; i < MAX_PARTICLES; i++) {
		if (mParticles[i].life < 0.0f) {
			Emit(i, mParticles[i].velocity.x, mParticles[i].velocity.y, mParticles[i].velocity.z, mParticles[i].constLife);
		}

		if (mParticles[i].life > 0.0f) {
			mParticles[i].position += (mParticles[i].velocity * dt);
		}
	}
}

void ParticleEmitter::RenderAll(const Camera& cam) {
	for (int i = 0; i < MAX_PARTICLES; i++) {
		if (mParticles[i].life > 0.0f) {
			mMesh->Translate(mParticles[i].position);
			mMesh->RenderMesh(*ShaderManager::Get()->getShdr("color"), false, true, true);
		}
	}
}

void ParticleEmitter::Emit(int index, float dx, float dy, float dz, float life) {
	mParticles[index].position = glm::vec3(x, y, z);
	mParticles[index].velocity = glm::vec3(dx, dy, dz);
	mParticles[index].life = life;
	mParticles[index].constLife = life;
	mParticles[index].color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

ParticleEmitter::~ParticleEmitter() {
	delete mMesh;
}