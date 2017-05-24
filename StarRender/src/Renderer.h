#pragma once
#include "Model.h"
#include "Material.h"
#include "Camera.h"
#include <map>
#include "Sprite2D.h"

class Renderer {
public:
	Renderer();
	~Renderer();
	void RenderSprite(GLTexture* texture, Camera& camera, float x, float y, float w, float h, float a);
	Sprite2D* CacheSprite(GLTexture* texture);
private:
	std::map<GLTexture*, Sprite2D*> mSpriteCache;
};