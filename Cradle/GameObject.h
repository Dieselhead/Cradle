#pragma once

#include <DirectXMath.h>

using namespace DirectX;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Render();
	virtual void Update(double total, double delta);

	virtual void Initialize();

private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scale;

	XMMATRIX m_transform;




};