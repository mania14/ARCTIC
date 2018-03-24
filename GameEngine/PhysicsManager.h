#pragma once

#include "PxPhysXConfig.h"
#include "PxPhysicsAPI.h"
#include "../CommonUtil/Singletone.h"

using namespace physx;

class PhysicsManager : public Singletone<PhysicsManager>
{
public:
	explicit PhysicsManager() {};
	virtual ~PhysicsManager() {};

public:
	void InitPhysics();
	void togglePvdConnection();
	void AsyncUpdate(float deltaTime);
	void WaitForAsync();
	void Release();

	PxScene* getScene() {
		return gScene;
	};
	PxPhysics* getPhysics() {
		return gPhysics;
	};
	PxCooking* getCooking() {
		return gCooking;
	};
	PxMaterial* getMaterial() {
		return gMaterial;
	};

private:
	PxDefaultAllocator				gAllocator;
	PxDefaultErrorCallback			gErrorCallback;

	PxFoundation*					gFoundation;
	PxPhysics*						gPhysics;

	PxDefaultCpuDispatcher*			gDispatcher;
	PxScene*						gScene;

	PxMaterial*						gMaterial;

	PxVisualDebuggerConnection*		gConnection;
	PxCooking*						gCooking;

	//ThreadWorker*					PxThread;
};