#include "Bone.h"
#include <glm/gtc/matrix_transform.hpp>

Bone::Bone() {
	name = "";
	id = -2;
}

Bone::Bone(Mesh* inMesh, unsigned int mId, std::string mName, aiMatrix4x4 inOMat) {
	id = mId;
	name = mName;
	offsetMatrix = AiToGLMMat4(inOMat);
	mesh = inMesh;

	parentBone = nullptr;
	node = nullptr;
}

Bone::Bone(Mesh* inMesh, unsigned int mId, std::string mName, glm::mat4 inOMat) {
	id = mId;
	name = mName;
	offsetMatrix = inOMat;
	mesh = inMesh;

	parentBone = nullptr;
	node = nullptr;
}

glm::mat4 Bone::GetParentTransforms() {
	Bone* b = parentBone;

	std::vector<glm::mat4> mats;

	while(b != nullptr) {
		glm::mat4 tmpMat = AiToGLMMat4(b->node->mTransformation);
		mats.push_back(tmpMat);

		b = b->parentBone; //Go up the hierarchy
	}

	glm::mat4 concatTransforms;
	for (int i = mats.size() - 1; i >= 0; i--) {
		concatTransforms *= mats[i];
	}
	return concatTransforms;
}

unsigned int Bone::FindPosition(float time) {
	for (unsigned int i = 0; i < animNode->mNumPositionKeys - 1; i++) {
		if (time < (float)animNode->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}
	return 0;
}

unsigned int Bone::FindRotation(float time) {
	for (unsigned int i = 0; i < animNode->mNumRotationKeys - 1; i++) {
		if (time < (float)animNode->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}
	return 0;
}

glm::vec3 Bone::CalcInterpolatedPosition(float time) {
	if (animNode->mNumPositionKeys == 1) {
		aiVector3D assimpval = animNode->mPositionKeys[0].mValue;
		glm::vec3 val(assimpval.x, assimpval.y, assimpval.z);
		return val;
	}

	unsigned int PosIndex = FindPosition(time);
	unsigned int NextPosIndex = PosIndex + 1;

	float dt = animNode->mPositionKeys[NextPosIndex].mTime - animNode->mPositionKeys[PosIndex].mTime;

	float factor = (time - (float)animNode->mPositionKeys[PosIndex].mTime) / dt;

	const aiVector3D& StartPos = animNode->mPositionKeys[PosIndex].mValue;
	const aiVector3D& EndPos = animNode->mPositionKeys[NextPosIndex].mValue;

	aiVector3D val = EndPos - StartPos;
	aiVector3D rval = StartPos + factor * val;
	rval = rval.Normalize();
	return (glm::vec3(rval.x, rval.y, rval.z));
}

glm::quat Bone::CalcInterpolatedRotation(float time) {
	if (animNode->mNumRotationKeys == 1)
	{
		aiQuaternion assimp_val = animNode->mRotationKeys[0].mValue;
		glm::quat val(assimp_val.w, assimp_val.x, assimp_val.y, assimp_val.z);
		return val;
	}

	unsigned int RotationIndex = FindRotation(time);
	unsigned int NextRotationIndex = (RotationIndex + 1);

	float DeltaTime = animNode->mRotationKeys[NextRotationIndex].mTime - animNode->mRotationKeys[RotationIndex].mTime;
	float Factor = (time - (float)animNode->mRotationKeys[RotationIndex].mTime) / DeltaTime;

	const aiQuaternion& StartRotationQ = animNode->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = animNode->mRotationKeys[NextRotationIndex].mValue;

	aiQuaternion val;
	aiQuaternion::Interpolate(val, StartRotationQ, EndRotationQ, Factor);
	val = val.Normalize();

	glm::quat rv(val.w, val.x, val.y, val.z);
	return rv;
}

unsigned int Bone::FindScaling(float time) {
	for (unsigned int i = 0; i < animNode->mNumScalingKeys - 1; i++) {
		if (time < (float)animNode->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}
	return 0;
}

glm::vec3 Bone::CalcInterpolatedScaling(float time) {
	if (animNode->mNumScalingKeys == 1) {
		aiVector3D assimpval = animNode->mScalingKeys[0].mValue;
		glm::vec3 val(assimpval.x, assimpval.y, assimpval.z);
		return val;
	}

	unsigned int PosIndex = FindScaling(time);
	unsigned int NextPosIndex = PosIndex + 1;

	float dt = animNode->mScalingKeys[NextPosIndex].mTime - animNode->mScalingKeys[PosIndex].mTime;

	float factor = (time - (float)animNode->mScalingKeys[PosIndex].mTime) / dt;

	const aiVector3D& StartPos = animNode->mScalingKeys[PosIndex].mValue;
	const aiVector3D& EndPos = animNode->mScalingKeys[NextPosIndex].mValue;

	aiVector3D val = EndPos - StartPos;
	aiVector3D rval = StartPos + factor * val;
	rval = rval.Normalize();
	return (glm::vec3(rval.x, rval.y, rval.z));
}

void Bone::UpdateKeyframeTransform(float time) {

	if (animNode == nullptr) {
		return;
	}

	pos = CalcInterpolatedPosition(time);
	rot = CalcInterpolatedRotation(time);
	scale = CalcInterpolatedScaling(time);

	glm::mat4 mat, transMat, rotMat, scaleMat;
	mat = glm::mat4(1.0f);
	transMat = glm::translate(mat, pos);
	rotMat = glm::mat4_cast(rot);
	scaleMat = glm::scale(mat, scale);

	mat = transMat * rotMat * scaleMat;

	node->mTransformation = GLMMat4ToAi(mat);
}