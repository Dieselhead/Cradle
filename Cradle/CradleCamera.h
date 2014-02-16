#ifndef _CRADLECAMERA_
#define _CRADLECAMERA_

#pragma unmanaged
#include <DirectXMath.h>
#pragma unmanaged
#include <d3d11.h>

class CradleGame;


class CradleCamera
{

public:
	struct cbTransform
	{
		DirectX::XMFLOAT4X4 WVP;
		// DirectX::XMFLOAT4X4 World;
		DirectX::XMFLOAT4X4 View;
		DirectX::XMFLOAT4X4 Projection;
		DirectX::XMFLOAT4X4 ViewProj;
	};

	cbTransform bufferObject;


	CradleCamera(CradleGame* game);
	virtual ~CradleCamera();

	DirectX::XMFLOAT4X4 m_mWorld;
	DirectX::XMFLOAT4X4 m_mView;
	DirectX::XMFLOAT4X4 m_mProjection;
	DirectX::XMFLOAT4X4 m_mWVP;
	
	DirectX::XMFLOAT4X4 m_mTransform;
	DirectX::XMFLOAT4X4 m_mRotation;

	DirectX::XMFLOAT3 m_vPosition;
	DirectX::XMFLOAT3 m_vRotation;
	DirectX::XMFLOAT3 m_vScale;

	DirectX::XMFLOAT3 m_vForward;
	DirectX::XMFLOAT3 m_vUp;
	DirectX::XMFLOAT3 m_vRight;

	DirectX::XMFLOAT3 FORWARD;
	DirectX::XMFLOAT3 UP;
	DirectX::XMFLOAT3 RIGHT;

	ID3D11Buffer* m_constantBuffer;


	void Initialize(float FOV, float aspectRatio, float nearClip, float farClip);
	void Update();
	void UpdateControls();


	CradleGame* m_game;


	const float m_Speed = 10.0f;
};

#endif