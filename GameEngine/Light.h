#include "Component.h"
#include "../System/ConstBufferDesc.h"

class Light : public Component
{
public:
	const static COMPONENTIDTYPE COMPONENTID = LIGHT_COMPONENT;

public:
	enum LightType
	{
		DIRECTINAL_LIGHT,
		POINT_LIGHT,
		CAPSULE_LIGHT,
		LIGHT_COUNT
	};

public:
	explicit Light();
	virtual ~Light();

public:
	virtual int				Init();
	virtual int				Release();
	COMPONENTIDTYPE GetComponentID() override {
		return COMPONENTID;
	};

	void					SetAmbient(const XMFLOAT4 ambient) {
		_LightBuffer._ambient = ambient;
	};
	void					SetSpecular(const XMFLOAT4 specular) {
		_LightBuffer._specular = specular;
	};
	void					SetDiffuse(const XMFLOAT4 diffuse) {
		_LightBuffer._diffuse = diffuse;
	};
	void					SetAttenuation(XMFLOAT3 attenuation) {
		_LightBuffer._attenuation = attenuation;
	};
	void					SetSpot(float spot) {
		_LightBuffer._spot = spot;
	};
	void					SetPosition(XMFLOAT3 position) {
		_LightBuffer._position = position;
	};
	void					SetDirection(XMFLOAT3 direction) {
		_LightBuffer._direction = direction;
	};
	void					SetRange(float range) {
		_LightBuffer._range = range;
	};
	void					SetLightType(LightType lighttype)
	{
		_eLightType = lighttype;
	}
	LightType				GetLightType()
	{
		return _eLightType;
	}
	const CBufferLight&		getLightBuffer() {
		return _LightBuffer;
	};

private:
	CBufferLight			_LightBuffer;
	LightType				_eLightType;
};