#include "PhysWorld.h"

/*
void PhysWorld::Init() {
	mCollisionConfig = new btDefaultCollisionConfiguration();
	mCollisionDispatch = new btCollisionDispatcher(mCollisionConfig);
	mPairCache = new btDbvtBroadphase();
	mSolver = new btSequentialImpulseConstraintSolver();
	mWorld = new btDiscreteDynamicsWorld(mCollisionDispatch, mPairCache, mSolver, mCollisionConfig);
}

void PhysWorld::Update(float dt) {
	
}

void PhysWorld::Close() {
	delete mCollisionConfig;
	delete mWorld;
}

btDiscreteDynamicsWorld* PhysWorld::getWorld() {
	return mWorld;
}*/