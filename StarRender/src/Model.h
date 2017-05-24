#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Bone.h"

class Model {
public:
	~Model();
	void Load(const char* path);
	void Draw(GLShader& shader, bool ForwardTextures = true, bool UseIndexing = true, bool ForceNoAnim = false);

	void AddMesh(Mesh* m);

	void Translate(glm::vec3 position);
	void Rotate(float angle, glm::vec3 axis);
	void Scale(glm::vec3 mod);

	void AddAnimation(Animation& anim);
	Animation* FindAnimation(std::string name);
	void PlayAnimation(Animation& anim, bool loop = false, bool reset_to_start = false);
	void StopAnimating();

	Mesh* GetMesh(int index) { return mMeshes[index]; }
private:
	std::vector<Mesh*> mMeshes;
	std::string directory;

	std::vector<aiNode*> ai_nodes;
	std::vector<aiNodeAnim*> ai_nodes_anim;

	void AnimNodeProcess(const aiScene* scene);

	glm::mat4 globalInverseTransform;

	bool mHasAnimation;

	std::vector<Bone*> bones;
	Skeleton* skeleton;
	std::vector<Animation> animations;

	Bone* FindBone(std::string name);
	int FindBoneID(std::string name);
	aiNode* FindAiNode(std::string name);
	aiNodeAnim* FindAiNodeAnim(std::string name);

	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
};