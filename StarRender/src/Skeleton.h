#pragma once
#include "Bone.h"

class Skeleton {
public:
	std::vector<Bone*> bones;
	glm::mat4 globalInverseTransform;
	std::vector<glm::mat4> boneMats;

	float time, startTime, endTime;

	Animation* activeAnimation;
	Animation* idleAnimation;

	bool animPlay, animLoop;

	Skeleton();
	~Skeleton();
	Skeleton(std::vector<Bone*> nBones, glm::mat4 nGIT);

	void Init(std::vector<Bone*> nBones, glm::mat4 nGIT);

	Bone* FindBone(std::string name);

	void UpdateBoneMatsVector();
	void Update(float dt);

	void SetIdleAnimation(Animation* nAnim);
	void StopAnimating();
	void PlayAnimation(Animation& anim, bool loop, bool reset_to_start);
};