#include "Model.h"
#include <glad/glad.h>
#include <iostream>

Model::~Model() {
	for (int i = 0; i < mMeshes.size(); i++) {
		delete mMeshes[i];
	}
}

void Model::Load(const char* path) {
	loadModel(path);
}

void Model::Draw(GLShader& shader, bool ForwardTextures, bool UseIndexing, bool ForceNoAnim) {
	for (int i = 0; i < mMeshes.size(); i++) {
		mMeshes[i]->RenderMesh(shader, UseIndexing, ForwardTextures, ForceNoAnim);
	}
}

//TODO(nick): Refactor this, it's not working atm
void Model::loadModel(std::string path) {
	//Use assimp to load a mesh file
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, NULL);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	AnimNodeProcess(scene);
	globalInverseTransform = glm::inverse(AiToGLMMat4(scene->mRootNode->mTransformation));
	processNode(scene->mRootNode, scene);

	for (int i = 0; i < scene->mNumMeshes; i++) {
		//Load bone data
		for (int j = 0; j < scene->mMeshes[i]->mNumBones; j++) {
			std::string bName = scene->mMeshes[i]->mBones[j]->mName.data;
			glm::mat4 bMat = glm::transpose(AiToGLMMat4(scene->mMeshes[i]->mBones[j]->mOffsetMatrix));

			std::cout << "Bone " << j << " " << bName << std::endl;

			Bone* nBone = new Bone(mMeshes[i], j, bName, bMat);

			nBone->node = FindAiNode(bName);
			nBone->animNode = FindAiNodeAnim(bName);

			if (nBone->animNode == nullptr) {
				std::cout << "No animations found for " << bName << std::endl;
			}

			

			//Fill out the children bones
			auto bnode = FindAiNode(bName);
			if (bnode) {
				std::string pName = bnode->mParent->mName.data;

				Bone* pBone = FindBone(pName);

				if (FindBone(pName) == nullptr) {
					std::cout << "Parent bone for " << bName << " does not exist (null pointer)" << std::endl;
				}
				else {
					nBone->parentBone = pBone;
				}

				bones.push_back(nBone);
			}
		}
	}

	if (mMeshes.size() > 0) {
		skeleton = new Skeleton(bones, globalInverseTransform);
		for (int i = 0; i < mMeshes.size(); i++) {
			if (bones.size() > 0) {
				mMeshes[i]->setSkeleton(skeleton);
			}
		}
	}
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	ai_nodes.push_back(node);

	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		mMeshes.push_back(processMesh(mesh, scene));
	}

	for (GLuint i = 0; i < node->mNumChildren; i++) {
		//Process child nodes
		processNode(node->mChildren[i], scene);
	}
}

void Model::AnimNodeProcess(const aiScene* scene) {
	if (scene->mNumAnimations == 0) {
		return;
	}

	for (int i = 0; i < scene->mAnimations[0]->mNumChannels; i++) {
		ai_nodes_anim.push_back(scene->mAnimations[0]->mChannels[i]);
	}
}

void Model::AddMesh(Mesh* m) {
	mMeshes.push_back(m);
}

//TODO(nick): Find out why something goes terribly wrong here
Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	Mesh* m = new Mesh();
	glm::vec3 position, normal, tangent, bitangent;
	glm::vec2 uvs;

	std::vector<VertexStruct> data;
	data.resize(mesh->mNumVertices);

	for (GLuint u = 0; u < mesh->mNumVertices; u++) {
		position.x = mesh->mVertices[u].x;
		position.y = mesh->mVertices[u].y;
		position.z = mesh->mVertices[u].z;

		normal.x = mesh->mNormals[u].x;
		normal.y = mesh->mNormals[u].y;
		normal.z = mesh->mNormals[u].z;

		if (mesh->mTextureCoords[0]) {
			uvs.x = mesh->mTextureCoords[0][u].x;
			uvs.y = mesh->mTextureCoords[0][u].y;
		}
		else {
			uvs = glm::vec2(0.0f, 0.0f);
		}

		m->AddVertex(position, uvs, normal);
	}

	if (mesh->HasFaces()) {
		for (GLuint j = 0; j < mesh->mNumFaces; j++) {
			aiFace face = mesh->mFaces[j];
			for (GLuint k = 0; k < face.mNumIndices; k++) {
				m->AddIndex(face.mIndices[k]);
			}
		}
	}


	if (mesh->HasBones()) {
		int WEIGHTS_PER_VERTEX = 4;

		int boneArraysSize = mesh->mNumVertices*WEIGHTS_PER_VERTEX;

		std::vector<int> boneIDs;
		boneIDs.resize(boneArraysSize);

		std::vector<float> boneWeights;
		boneWeights.resize(boneArraysSize);

		//Make sure the actual data is the right size
		VertexData* mdat = m->getVertexData();
		mdat->boneids.resize(mesh->mNumVertices);
		mdat->weights.resize(mesh->mNumVertices);


		for (int i = 0; i<mesh->mNumBones; i++) {

			aiBone* aiBone = mesh->mBones[i];

			for (int j = 0; j<aiBone->mNumWeights; j++) {
				aiVertexWeight weight = aiBone->mWeights[j];

				unsigned int vertexStart = weight.mVertexId * WEIGHTS_PER_VERTEX;

				for (int k = 0; k<WEIGHTS_PER_VERTEX; k++) {

					if (boneWeights.at(vertexStart + k) == 0) {
						boneWeights.at(vertexStart + k) = weight.mWeight;
						boneIDs.at(vertexStart + k) = i;

						data[weight.mVertexId].bone_ids[k] = i;
						data[weight.mVertexId].weights[k] = weight.mWeight;
						break;
					}
				}
			}
		}

		for (int i = 0; i < data.size(); i++) {

			glm::ivec4 bones;
			glm::vec4 weights;

			bones = glm::ivec4(data[i].bone_ids[0], data[i].bone_ids[1], data[i].bone_ids[2], data[i].bone_ids[3]);
			weights = glm::vec4(data[i].weights[0], data[i].weights[1], data[i].weights[2], data[i].weights[3]);

			m->DefineBoneAndWeights(i, bones, weights);
		}
	}

	m->GenerateMesh();
	return m;
}

void Model::Translate(glm::vec3 position) {
	for (int i = 0; i < mMeshes.size(); i++) {
		mMeshes[i]->Translate(position);
	}
}

void Model::Rotate(float angle, glm::vec3 axis) {
	for (int i = 0; i < mMeshes.size(); i++) {
		mMeshes[i]->Rotate(angle, axis);
	}
}

void Model::Scale(glm::vec3 mod) {
	for (int i = 0; i < mMeshes.size(); i++) {
		mMeshes[i]->Scale(mod);
	}
}

Bone* Model::FindBone(std::string name) {
	for (int i = 0; i < bones.size(); i++) {
		if (bones[i]->name == name) {
			return bones[i];
		}
	}
	return nullptr;
}

aiNode* Model::FindAiNode(std::string name) {
	for (int i = 0; i < ai_nodes.size(); i++) {
		if (ai_nodes[i]->mName.data == name) {
			return ai_nodes[i];
		}
	}
	return 0;
}

aiNodeAnim* Model::FindAiNodeAnim(std::string name) {
	for (int i = 0; i < ai_nodes_anim.size(); i++) {
		if (ai_nodes_anim[i]->mNodeName.data == name) {
			return ai_nodes_anim[i];
		}
	}
	return 0;
}

int Model::FindBoneID(std::string name) {
	for (int i = 0; i < bones.size(); i++) {
		if (bones[i]->name == name) {
			return i;
		}
	}
	return -1;
}

void Model::AddAnimation(Animation& anim) {
	animations.push_back(anim);
}

Animation* Model::FindAnimation(std::string name) {
	for (int i = 0; i < animations.size(); i++) {
		if (animations[i].name == name) {
			return &animations[i];
		}
	}
	return nullptr;
}

void Model::PlayAnimation(Animation& anim, bool loop, bool reset_to_start) {
	skeleton->PlayAnimation(anim, loop, reset_to_start);
	for (int i = 0; i < mMeshes.size(); i++) {
		mMeshes[i]->skeleton->PlayAnimation(anim, loop, reset_to_start);
	}
}

void Model::StopAnimating() {
	skeleton->StopAnimating();
}