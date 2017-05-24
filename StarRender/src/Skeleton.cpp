#include "Skeleton.h"

Skeleton::Skeleton() {
	time = startTime = endTime = 0;
	activeAnimation = nullptr;
	animLoop = false;
}

Skeleton::Skeleton(std::vector<Bone*> nBones, glm::mat4 nGIT) {
	Init(nBones, nGIT);
}

Skeleton::~Skeleton() {
	for (int i = 0; i < bones.size(); i++) {
		delete bones[i];
	}
}

void Skeleton::Init(std::vector<Bone*> nBones, glm::mat4 nGIT) {
	bones = nBones;
	globalInverseTransform = nGIT;

	time = startTime = endTime = 0;
	activeAnimation = nullptr;
	idleAnimation = nullptr;
	animLoop = false;

	for (int i = 0; i < bones.size(); i++) {
		bones[i]->parentSkeleton = this;
	}
}

Bone* Skeleton::FindBone(std::string name) {
	for (int i = 0; i < bones.size(); i++)
	{
		if (bones[i]->name == name)
			return bones[i];
	}
	return nullptr;
}

void Skeleton::UpdateBoneMatsVector() {
	if (bones.size() == 0) {
		return;
	}

	boneMats.clear();

	for (int i = 0; i < 100; i++) {
		if (i > bones.size() - 1) {
			boneMats.push_back(glm::mat4(1.0f));
		}
		else {
			glm::mat4 concatTrans = globalInverseTransform * (bones[i]->GetParentTransforms() * AiToGLMMat4(bones[i]->node->mTransformation)) * bones[i]->offsetMatrix;
			boneMats.push_back(concatTrans);
		}
	}
}

void Skeleton::Update(float dt) {
	UpdateBoneMatsVector();
	if (!animPlay) {
		return;
	}

	time += dt;

	if (time < startTime) {
		time = startTime;
	}

	if (time > endTime) {
		if (animLoop) {
			time = startTime;
		}
		else {
			StopAnimating();
		}
	}

	for (int i = 0; i < bones.size(); i++) {
		bones[i]->UpdateKeyframeTransform(time);
	}
}

void Skeleton::SetIdleAnimation(Animation* nAnim) {
	idleAnimation = nAnim;
}

void Skeleton::StopAnimating() {
	time = endTime;
	activeAnimation = nullptr;
	animPlay = false;
}

void Skeleton::PlayAnimation(Animation& anim, bool loop, bool reset_to_start) {
	if (activeAnimation != nullptr)
	{
		if (anim.priority < activeAnimation->priority) {
			activeAnimation = &anim;
		} else {
			return;
		}
	} else {
		activeAnimation = &anim;
	}

	startTime = activeAnimation->startTime;
	endTime = activeAnimation->endTime;

	animPlay = true;
	animLoop = loop;

	if (reset_to_start) {
		time = activeAnimation->startTime;
	}
}