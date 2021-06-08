#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <SDL.h>

const float CONST_MSPEED = 0.005f;

Camera::Camera() : mProjectionMatrix(1.0f), mViewMatrix(1.0f), m_hAngle(0.0f), m_vAngle(0.0f) {}

Camera::~Camera() {

}

void Camera::defineVAngle(float a) {
	m_vAngle = a;
}

void Camera::defineHAngle(float a) {
	m_hAngle = a;
}

void Camera::setPosition(glm::vec3 nPos) {
	mCameraPosition = nPos;
}

void Camera::toggleMouselook() {
	mUseMS = !mUseMS;
}

void Camera::init(float w, float h) {
	mUseMS = true;
	mWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	mCameraDirection = glm::vec3(cos(m_vAngle) * sin(m_hAngle), sin(m_vAngle), cos(m_vAngle) * cos(m_hAngle));
	mProjectionMatrix = glm::perspective(glm::radians(mFov), w / h, 0.1f, 1000.0f);
	mViewMatrix = glm::lookAt(mCameraPosition, mCameraPosition + mCameraDirection, mWorldUp);
	mWidth = w;
	mHeight = h;
	mOrthoMatrix = glm::ortho(0.0f, w, h, 0.0f);
}

glm::mat4 Camera::getMatrix(std::string type) {
	if (type == "projection") {
		return mProjectionMatrix;
	}

	if (type == "view") {
		return mViewMatrix;
	}

	if (type == "ortho") {
		return mOrthoMatrix;
	}

	return glm::mat4(1.0f);
}

void Camera::updateVectors() {
	mCameraRight = glm::normalize(glm::cross(mCameraDirection, mWorldUp));
	mCameraUp = glm::cross(mCameraDirection, mCameraRight);
}

void Camera::update(float dt, InputManager* input) {
	if (mUseMS) {
		//Mouse look
		float dx, dy;
		int mx, my;
		mx = input->mouse_x();
		my = input->mouse_y();
		dx = std::fmod(mx - (mWidth / 2), 360.0f);
		dy = static_cast<float>(my - (mHeight / 2));
		float sensitivity = 0.01f;

		dx *= sensitivity;
		dy *= sensitivity;

		m_hAngle -= (dx)* dt;
		m_vAngle -= (dy)* dt;

		if (m_vAngle < -80.0f) {
			m_vAngle = -80.0f;
		}

		if (m_vAngle > 80.0f) {
			m_vAngle = 80.0f;
		}

		SDL_WarpMouseInWindow(NULL, mWidth / 2, mHeight / 2);
		mCameraDirection = glm::vec3(cos(glm::radians(m_vAngle)) * sin(glm::radians(m_hAngle)), sin(glm::radians(m_vAngle)), cos(glm::radians(m_vAngle)) * cos(glm::radians(m_hAngle)));
		mViewMatrix = glm::lookAt(mCameraPosition, mCameraPosition + mCameraDirection, mWorldUp);
	}
	else {
		mViewMatrix = glm::lookAt(mCameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), mWorldUp);
	}
	
	mProjectionMatrix = glm::perspective(glm::radians(mFov), mWidth / mHeight, 0.1f, 1000.0f);
}

void Camera::defineFOV(float fov) {
	mFov = fov;
}

void Camera::definePosition(float x, float y, float z) {
	mCameraPosition = glm::vec3(x, y, z);
}