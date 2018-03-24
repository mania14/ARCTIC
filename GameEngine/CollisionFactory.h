#pragma once
#include "../CommonUtil/Singletone.h"
#include "../System/VertexDesc.h"
#include "PhysicsManager.h"

struct CollisionDesc
{
	
};

class Collision;
class Mesh;
class CollisionFactory : public Singletone<CollisionFactory>
{
public:
	CollisionFactory::CollisionFactory() {};
	CollisionFactory::~CollisionFactory() {};

public:
	bool Init();
	bool Release();
	
	bool CreateConvexMeshCollision(std::vector<physx::PxVec3>& input, Collision* output);
	bool CreateConvexMeshCollision(std::vector<Vertex_Color>& input, Collision* output);
	//bool CreateTriangleMeshCollision(std::vector<physx::PxVec3>& input, Collision* output);
	bool CreateTriangleMeshCollision(Mesh& input, Collision* output);
};