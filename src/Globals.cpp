#include "Globals.h"

Globals* Globals::mInst = nullptr;

Globals* Globals::Get() {
	if (mInst == nullptr) {
		mInst = new Globals();
	}
	return mInst;
}