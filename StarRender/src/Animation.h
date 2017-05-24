#pragma once
#include <glm/glm.hpp>
#include <string>

const float FRAME_MULT = 1.0f / 60.0f;

static float TimeToFrame(float time) {
	return time / FRAME_MULT;
}

static glm::vec2 FramesToTime(glm::vec2 frames) {
	return frames * FRAME_MULT;
}

class Animation {
public:
	std::string name;
	float startTime, endTime;
	int priority;

	Animation();
	Animation(std::string nName, glm::vec2 times, int nPriority);
};