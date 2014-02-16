#ifndef _CAMERA_
#define _CAMERA_

#include <d3d11.h>
#include <DirectXMath.h>

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void Initialize(float fov, float aspectRatio, float nearClip, float farClip);
	void Update();



	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotation;
	DirectX::XMFLOAT3 m_scale;
	
	DirectX::XMMATRIX m_transform;

	DirectX::XMMATRIX m_view;
	DirectX::XMMATRIX m_projection;
	DirectX::XMMATRIX m_world;

	DirectX::XMFLOAT3 m_up;
	DirectX::XMFLOAT3 m_forward;
	DirectX::XMFLOAT3 m_right;
	DirectX::XMFLOAT3 m_target;

	DirectX::XMFLOAT3 m_zero;

	DirectX::XMMATRIX m_wvp;

};
#endif