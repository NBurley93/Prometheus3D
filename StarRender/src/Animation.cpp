#include "Animation.h"

Animation::Animation() {
	startTime = endTime = priority = 0;
}

Animation::Animation(std::string nName, glm::vec2 times, int nPriority) {
	name = nName;
	startTime = times.x;
	endTime = times.y;
	priority = nPriority;
}