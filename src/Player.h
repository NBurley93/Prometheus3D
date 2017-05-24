#pragma once
#include <Camera.h>

class Player {
public:
	void Init(glm::vec3 pos);
	void Update(float dt, InputManager* input);

	glm::vec3 getPosition() { return mPosition; }

	bool Moving() { return mIsMoving; }
	bool Sprinting() { return mIsSprinting; }
	bool Crouching() { return mIsCrouching; }

	void attachCamera(Camera* cam);
private:
	bool mIsCrouching, mIsSprinting, mIsMoving;
	bool mIsJumped;
	Camera* mPlayerCam;
	glm::vec3 mPosition;
	glm::vec3 mVelocity;
};