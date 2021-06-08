#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <InputManager.h>

class Camera {
public:
	Camera();
	~Camera();

	void init(float w, float h);
	glm::mat4 getMatrix(std::string type);

	void updateVectors();
	void update(float dt, InputManager* input);

	void defineFOV(float fov);
	void definePosition(float x, float y, float z);
	void defineVAngle(float a);
	void defineHAngle(float a);

	float get_hAngle() { return m_hAngle; }
	float get_vAngle() { return m_vAngle; }
	float get_fov() { return mFov; }

	glm::vec3 getPosition() { return mCameraPosition; }
	glm::vec2 getAngles() { return glm::vec2(m_hAngle, m_vAngle); }

	glm::vec3 getDirection() { return mCameraDirection; }
	glm::vec3 getRight() { return mCameraRight; }

	void setPosition(glm::vec3 nPos);

	void toggleMouselook();

private:
	glm::vec3 mWorldUp, mCameraUp, mCameraRight, mCameraDirection, mCameraPosition;

	float mFov, m_hAngle, m_vAngle, mWidth, mHeight;
	bool mUseMS;

	glm::mat4 mProjectionMatrix, mViewMatrix, mOrthoMatrix;
};