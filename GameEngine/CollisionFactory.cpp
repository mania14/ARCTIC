#include "stdafx.h"
#include "CollisionFactory.h"
#include "Collision.h"
#include "Mesh.h"
#include <vector>

bool CollisionFactory::Init()
{
	return false;
}

bool CollisionFactory::Release()
{
	return false;
}

bool CollisionFactory::CreateConvexMeshCollision(std::vector<Vertex_Color>& input, Collision* output)
{
	std::vector<PxVec3> convexVerts;
	convexVerts.resize(input.size());

	for (int i = 0; i < input.size(); ++i)
	{
		convexVerts[i].x = input[i].Pos.x;
		convexVerts[i].y = input[i].Pos.y;
		convexVerts[i].z = input[i].Pos.z;
	}

	CreateConvexMeshCollision(convexVerts, output);

	return true;
}

bool CollisionFactory::CreateTriangleMeshCollision(Mesh & input, Collision * output)
{
	Mesh* pMesh = input.vecChildMesh[0];
	std::vector<PxVec3> PxVerts;
	PxVerts.resize(pMesh->vecVertex.size());

	for (int i = 0; i < pMesh->vecVertex.size(); ++i)
	{
		PxVerts[i].x = pMesh->vecVertex[i].Pos.x;
		PxVerts[i].y = pMesh->vecVertex[i].Pos.y;
		PxVerts[i].z = pMesh->vecVertex[i].Pos.z;
	}

	PxTriangleMeshDesc triangleMeshDesc;
	triangleMeshDesc.points.count = (int)pMesh->vecVertex.size();
	triangleMeshDesc.points.stride = sizeof(PxVec3);
	triangleMeshDesc.points.data = &PxVerts[0];
	triangleMeshDesc.triangles.count = (int)pMesh->vecIndics.size();
	triangleMeshDesc.triangles.stride = 3 * sizeof(PxU32);
	triangleMeshDesc.triangles.data = &pMesh->vecIndics[0];

	PxDefaultMemoryOutputStream buf;
	PxTriangleMesh* triangleMesh = PhysicsManager::This().getCooking()->createTriangleMesh(triangleMeshDesc, PhysicsManager::This().getPhysics()->getPhysicsInsertionCallback());
	if (!triangleMesh)
		return NULL;

	//PxDefaultMemoryInputData inputStream(buf.getData(), buf.getSize());
	//PxTriangleMesh* triangleMesh = PhysicsManager::This().getPhysics()->createTriangleMesh(inputStream);

	/*if (!triangleMesh)
	{
		return NULL;
	}*/

	PxRigidStatic* pRigid = PhysicsManager::This().getPhysics()->createRigidStatic(PxTransform(PxVec3(0.f, 10.f, 0.f)));
	PxShape* pShape = pRigid->createShape(PxTriangleMeshGeometry(triangleMesh), *PhysicsManager::This().getMaterial());

	if(!pShape)
	{
		return NULL;
	}
	pRigid->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	output->RigidBody = pRigid;

	PhysicsManager::This().getScene()->addActor(*pRigid);

	return false;
}

bool CollisionFactory::CreateConvexMeshCollision(std::vector<PxVec3>& input, Collision* output)
{	
	PxConvexMeshDesc convexMeshDesc;
	convexMeshDesc.points.count = (int)input.size();
	convexMeshDesc.points.stride = sizeof(PxVec3);
	convexMeshDesc.points.data = &input[0];
	convexMeshDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX | PxConvexFlag::eINFLATE_CONVEX;

	PxDefaultMemoryOutputStream buf;
	if (!PhysicsManager::This().getCooking()->cookConvexMesh(convexMeshDesc, buf))
		return NULL;

	PxDefaultMemoryInputData inputStream(buf.getData(), buf.getSize());
	PxConvexMesh* convexMesh = PhysicsManager::This().getPhysics()->createConvexMesh(inputStream);

	PxShape* pShape = PhysicsManager::This().getPhysics()->createShape(PxConvexMeshGeometry(convexMesh), *PhysicsManager::This().getMaterial());
	PxRigidStatic* pRigid = PhysicsManager::This().getPhysics()->createRigidStatic(PxTransform(PxVec3(0.f, 10.f, 0.f)));
	physx::PxRigidActor*  RigidBody = pRigid;
	pRigid->attachShape(*pShape);
	pRigid->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	//output->vecRigidBody.push_back(pRigid);
	output->RigidBody = pRigid;

	PhysicsManager::This().getScene()->addActor(*pRigid);

	pShape->release();

	return false;
}
