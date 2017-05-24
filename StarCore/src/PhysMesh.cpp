#include "PhysMesh.h"

btTriangleMesh* PhysMesh::getShape() {
	return mShape;
}

PhysMesh::PhysMesh() {
	mShape = new btTriangleMesh();
}

PhysMesh::~PhysMesh() {
	delete mShape;
}