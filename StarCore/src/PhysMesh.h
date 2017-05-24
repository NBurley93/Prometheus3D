#pragma once
#include <btBulletCollisionCommon.h>

class PhysMesh {
public:
	PhysMesh();
	~PhysMesh();
	btTriangleMesh* getShape();
private:
	btTriangleMesh* mShape;
};