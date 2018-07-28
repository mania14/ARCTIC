#pragma once
#include "Object.h"
#include "vector"

class Component;

class GameObject : public Object
{
public:
	explicit GameObject();
	virtual ~GameObject();

public:
	void	virtual Create();
	void	virtual Update();
	void	virtual Render() = 0;
	void	virtual Destroy();

	void	AddComponent(Component* pComponent);
	template <class T>
	T*		GetComponent( int index = 0)
	{
		// �ӽ� ���߿� �䷯�ݽ��� �ٲ���
		return !_vecComponent[T::COMPONENTID].empty() ? static_cast<T*>(_vecComponent[T::COMPONENTID][index]) : nullptr;
	}
	//void	DeleteComponent();

private:
	std::vector<std::vector<Component*>>	_vecComponent;


};
