#include "Renderer.h"

Renderer::Renderer() {
}

Renderer::~Renderer() {
	for (auto mit = mSpriteCache.begin(); mit != mSpriteCache.end(); mit++) {
		delete mit->second;
	}
	mSpriteCache.clear();
}

void Renderer::RenderSprite(GLTexture* texture, Camera& camera, float x, float y, float w, float h, float a) {
	Sprite2D* s = CacheSprite(texture);
	s->SetAlpha(a);
	s->Draw(camera, x, y, w, h);
}

Sprite2D* Renderer::CacheSprite(GLTexture* texture) {
	auto mit = mSpriteCache.find(texture);
	if (mit == mSpriteCache.end()) {
		Sprite2D* spr = new Sprite2D();
		spr->Init(texture);
		mSpriteCache[texture] = spr;
		return spr;
	}
	else {
		return mit->second;
	}
}