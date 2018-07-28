#pragma once
#include "../CommonUtil/Singletone.h"
#include "../System/RenderDevice.h"

struct Gizmo
{
	ID3D11Buffer*		  pPositionGizmoBuffer;
	ID3D11Buffer*		  pScaleGizmoBuffer;
	ID3D11Buffer*		  pRotateGizmoBuffer;
	ID3D11Buffer*		  pLineGizmoBuffer;
	int					  PositionGizmoCount;
	int					  ScaleGizmoCount;
	int					  LineGizmoCount;
};

class GameObject;
class Transform;
class ToolTransformManager : public Singletone<ToolTransformManager>
{
public:
	enum PickMode
	{
		TOOL_TRANSFORM_POSITION,
		TOOL_TRANSFORM_ROTATE,
		TOOL_TRANSFORM_SCALE,
		TOOL_TRANSFORM_COUNT
	};

	enum PickAxis
	{
		TOOL_TRANSFORM_AXIS_X,
		TOOL_TRANSFORM_AXIS_Y,
		TOOL_TRANSFORM_AXIS_Z,
		TOOL_TRANSFORM_AXIS_COUNT
	};

public:
	explicit ToolTransformManager();
	virtual ~ToolTransformManager();

public:
	void Init();
	void Update();
	void Render();

public:
	void setPickMode(PickMode pickmode) { CurrentMode = pickmode; };

private:
	PickMode			CurrentMode;
	PickAxis			CurrentAxis;
	GameObject*			_currentGameObject;
	acm::float3				_vecOldRay;
	bool				_isClicked;
	Gizmo*				_pGizmo;

	void				CreateGizmo();
	void				RenderGizmo(Transform* pTransform);
};