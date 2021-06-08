#include "Player.h"
#include <SDL_keycode.h>
#include "SoundManager.h"

const float CONST_SPEED = 0.1f;
const float CONST_SPRINT_SPEED = 0.4f;
const float CAM_HEIGHT = 65.0f;
const float JUMP_HEIGHT = 0.1f;
const float CONST_GRAVITY = 0.0001f;

void Player::Init(glm::vec3 pos) {
	mPosition = pos;
	mIsJumped = false;
}

void Player::Update(float dt, InputManager* input) {
	mPlayerCam->updateVectors();

	if (input->isKeyDown(SDLK_LCTRL)) {
		mIsCrouching = true;
	}
	else {
		mIsCrouching = false;
	}

	if (input->isKeyDown(SDLK_LSHIFT)) {
		mIsSprinting = true;
	}
	else {
		mIsSprinting = false;
	}

	float TRUE_SPEED = CONST_SPEED;
	if (mIsCrouching) {
		TRUE_SPEED /= 2;
	}
	else {
		if (mIsSprinting) {
			TRUE_SPEED = CONST_SPRINT_SPEED;
		}
	}

	glm::vec3 cdir = mPlayerCam->getDirection();
	glm::vec3 cright = mPlayerCam->getRight();
	if (input->isKeyDown(SDLK_w)) {
		mPosition += glm::normalize(glm::vec3(cdir.x, 0.0f, cdir.z)) * dt * TRUE_SPEED;
	}

	if (input->isKeyDown(SDLK_s)) {
		mPosition -= glm::normalize(glm::vec3(cdir.x, 0.0f, cdir.z)) * dt * TRUE_SPEED;
	}

	if (input->isKeyDown(SDLK_a)) {
		mPosition -= cright * dt * TRUE_SPEED;
	}

	if (input->isKeyDown(SDLK_d)) {
		mPosition += cright * dt * TRUE_SPEED;
	}

	if (input->isKeyDown(SDLK_w) || input->isKeyDown(SDLK_a) || input->isKeyDown(SDLK_s) || input->isKeyDown(SDLK_d)) {
		mIsMoving = true;
	}
	else
	{
		mIsMoving = false;
	}

	if (input->isKeyPressed(SDLK_SPACE)) {
		if (mPosition.y == 0.0f && !mIsCrouching) {
			mVelocity.y += JUMP_HEIGHT;
			mIsJumped = true;
		}
	}



	if (mPosition.y > 0.0f) {
		mVelocity.y -= CONST_GRAVITY * dt;
		mIsMoving = false;
	}

	if (mPosition.y < 0.0f) {
		mVelocity.y = 0.0f;
		mPosition.y = 0.0f;
		if (mIsJumped) {
			int rNumber = rand() % 4 + 1;
			SoundManager::Get()->playSFX("./data/sounds/landing/land" + std::to_string(rNumber) + ".wav");
		}
		mIsJumped = false;
	}

	float TRUE_HEIGHT = CAM_HEIGHT;
	if (mIsCrouching) {
		TRUE_HEIGHT /= 2;
	}

	mPosition += mVelocity * dt;
	glm::vec3 camPos = glm::vec3(mPosition.x, mPosition.y + TRUE_HEIGHT, mPosition.z);

	mPlayerCam->setPosition(camPos);
	mPlayerCam->update(dt, input);
}

void Player::attachCamera(Camera* cam) {
	mPlayerCam = cam;
}