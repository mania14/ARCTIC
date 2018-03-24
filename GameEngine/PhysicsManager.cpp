#include "stdafx.h"
#include "PhysicsManager.h"
#include "../System/ThreadWorker.h"

void PhysicsManager::InitPhysics()
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	if (!gFoundation)
	{
		MessageBoxA(NULL, "PxCreateFoundation failed!", "error", MB_OK);
		return;
	}
	PxProfileZoneManager* profileZoneManager = &PxProfileZoneManager::createProfileZoneManager(gFoundation);
	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, profileZoneManager);
	if (!gPhysics)
	{
		MessageBoxA(NULL, "PxCreatePhysics failed!", "error", MB_OK);
		return;
	}

	if (gPhysics->getPvdConnectionManager())
	{
		gPhysics->getVisualDebugger()->setVisualizeConstraints(true);
		gPhysics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_CONTACTS, true);
		gPhysics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
		gConnection = PxVisualDebuggerExt::createConnection(gPhysics->getPvdConnectionManager(), "127.0.0.1", 5425, 10);
	}

	//PxCookingParams params(PxTolerancesScale());
	//params.meshWeldTolerance = 0.001f;
	//params.meshPreprocessParams = PxMeshPreprocessingFlags(PxMeshPreprocessingFlag::eWELD_VERTICES | PxMeshPreprocessingFlag::eREMOVE_UNREFERENCED_VERTICES | PxMeshPreprocessingFlag::eREMOVE_DUPLICATED_TRIANGLES);
	gCooking = PxCreateCooking(PX_PHYSICS_VERSION, *gFoundation, PxCookingParams(PxTolerancesScale()));
	if (!gCooking)
	{
		MessageBoxA(NULL, "PxCreateCooking failed!", "error", MB_OK);
		return;
	}

	{
		//togglePvdConnection();
	}

	// create scene
	{
		PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
		gDispatcher = PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = gDispatcher;
		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
		gScene = gPhysics->createScene(sceneDesc);
	}

	gMaterial = gPhysics->createMaterial(0.0f, 0.0f, 0.0f);

	// create default plane
	{
		PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, PxPlane(0, 1, 0, 0), *gMaterial);
		gScene->addActor(*groundPlane);
	}

	/*PxRigidDynamic* dyn = gPhysics->createRigidDynamic(PxTransform(PxVec3(0.f, 2.5f, 0.f)));
	PxShape* shape1 = gPhysics->createShape(PxBoxGeometry(2.f, 0.2f, 0.1f), *gMaterial);
	PxShape* shape2 = gPhysics->createShape(PxBoxGeometry(0.2f, 2.f, 0.1f), *gMaterial);

	dyn->attachShape(*shape1);
	dyn->attachShape(*shape2);

	dyn->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);*/
	//dyn->setAngularVelocity(PxVec3(0.f, 0.f, 5.f));
	//dyn->setAngularDamping(0.f);

	//PxRigidStatic* st = gPhysics->createRigidStatic(PxTransform(PxVec3(0.f, 10.5f, -1.f)));
	//PxShape* shape3 = gPhysics->createShape(PxBoxGeometry(0.5f, 1.5f, 0.8f), *gMaterial);
	//st->attachShape(*shape3);

	//gScene->addActor(*dyn);
	//gScene->addActor(*st);

	


	//for (PxU32 i = 0; i < 5; i++)
	//{
	//	PxTransform t = PxTransform(PxVec3(0, 0, 0));
	//	PxShape* shape = gPhysics->createShape(PxBoxGeometry(2.0f, 2.0f, 2.0f), *gMaterial);
	//	for (PxU32 i = 0; i < 10; i++)
	//	{
	//		for (PxU32 j = 0; j < 10 - i; j++)
	//		{
	//			PxTransform localTm(PxVec3(PxReal(j * 2) - PxReal(10 - i), PxReal(i * 2 + 1), 0) * 2.0f);
	//			PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
	//			body->attachShape(*shape);
	//			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
	//			gScene->addActor(*body);
	//		}
	//	}
	//	shape->release();
	//}
}

void PhysicsManager::togglePvdConnection()
{
	if (!gPhysics->getPvdConnectionManager()) return;
	if (gPhysics->getPvdConnectionManager()->isConnected())
	{
		gPhysics->getPvdConnectionManager()->disconnect();
	}
	else
	{
		PxVisualDebuggerConnectionManager* pvd = gPhysics->getPvdConnectionManager();
		if (!pvd)
			return;

		//The connection flags state overall what data is to be sent to PVD.  Currently
		//the Debug connection flag requires support from the implementation (don't send
		//the data when debug isn't set) but the other two flags, profile and memory
		//are taken care of by the PVD SDK.

		//Use these flags for a clean profile trace with minimal overhead
		PxVisualDebuggerConnectionFlags theConnectionFlags(PxVisualDebuggerConnectionFlag::eDEBUG | PxVisualDebuggerConnectionFlag::ePROFILE | PxVisualDebuggerConnectionFlag::eMEMORY);
		//if (!mPvdParams.useFullPvdConnection)
		{
			theConnectionFlags = PxVisualDebuggerConnectionFlag::ePROFILE;
		}

		//Create a pvd connection that writes data straight to the filesystem.  This is
		//the fastest connection on windows for various reasons.  First, the transport is quite fast as
		//pvd writes data in blocks and filesystems work well with that abstraction.
		//Second, you don't have the PVD application parsing data and using CPU and memory bandwidth
		//while your application is running.
		//PxVisualDebuggerExt::createConnection(mPhysics->getPvdConnectionManager(), "c:\\temp.pxd2", theConnectionFlags);

		//The normal way to connect to pvd.  PVD needs to be running at the time this function is called.
		//We don't worry about the return value because we are already registered as a listener for connections
		//and thus our onPvdConnected call will take care of setting up our basic connection state.

		PxVisualDebuggerExt::createConnection(pvd, "127.0.0.1", 5425, 10, theConnectionFlags);
	}
}

void PhysicsManager::AsyncUpdate(float deltaTime)
{
	//PxThread->Start();
	gScene->simulate(deltaTime);
	gScene->fetchResults(true);
}

void PhysicsManager::WaitForAsync()
{
	//PxThread->Join();
}

void PhysicsManager::Release()
{
	PxProfileZoneManager* profileZoneManager = gPhysics->getProfileZoneManager();
	if (gConnection != NULL)
		gConnection->release();
	gPhysics->release();
	profileZoneManager->release();
	gFoundation->release();
	gCooking->release();
}
