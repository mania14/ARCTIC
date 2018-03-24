#include "stdafx.h"
#include "Component.h"
#include "GameObject.h"

GameObject::GameObject()
{
	Create();
}

GameObject::~GameObject()
{
	Destroy();
}

void GameObject::Create()
{
	_vecComponent.clear();
	_vecComponent.resize(END_COMPONENT);
}

void GameObject::Update()
{
	auto enditer = _vecComponent.end();

	for (auto iter = _vecComponent.begin(); iter != enditer; ++iter)
	{
		for (auto iter2 = (*iter).begin(); iter2 != (*iter).end(); ++iter2)
		{
			(*iter2)->Update();
		}
	}
}

void GameObject::Destroy()
{
	std::for_each(_vecComponent.begin(), _vecComponent.end(), [](std::vector<Component*>& pComponent)->void {
		for (auto iter = pComponent.begin(); iter != pComponent.end(); ++iter)
		{
			(*iter)->Release();
			delete (*iter);
		}
	});

	_vecComponent.clear();
}

void GameObject::AddComponent(Component * pComponent)
{
	if (pComponent)
	{
		_vecComponent[pComponent->GetComponentID()].push_back(pComponent);
	}
}
