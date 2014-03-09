#ifndef _CRADLEACTOR_
#define _CRADLEACTOR_

#include "BasicTimer.h"
#include <DirectXMath.h>
#include <string>
#include "CradleMesh.h"

// An object with a position in the World.
class CradleActor
{
public:
	CradleActor(CradleMesh* mesh);
	CradleActor(void);
	virtual ~CradleActor(void);

	virtual void Update(BasicTimer* time);
	virtual void Draw();
	virtual void Initialize();

	void SetMesh(CradleMesh* mesh);

public:
	DirectX::XMFLOAT4X4 m_mWorld;
	DirectX::XMFLOAT4X4 m_mTransform;
	DirectX::XMFLOAT4X4 m_mRotation;

	DirectX::XMFLOAT3 m_Offset;
	DirectX::XMFLOAT3 m_vPosition;
	DirectX::XMFLOAT3 m_vRotation;
	DirectX::XMFLOAT3 m_vScale;

	DirectX::XMFLOAT3 m_vForward;
	DirectX::XMFLOAT3 m_vRight;
	DirectX::XMFLOAT3 m_vUp;

	CradleMesh* m_mesh;
	float m_rotationRate;

};

#endif