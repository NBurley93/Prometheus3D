#include "LightManager.h"

LightManager* LightManager::mInst = nullptr;

LightManager* LightManager::Get() {
	if (mInst == nullptr) {
		mInst = new LightManager();
	}
	return mInst;
}