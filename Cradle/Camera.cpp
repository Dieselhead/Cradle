#include "Camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::Initialize(float fov, float aspectRatio, float nearClip, float farClip)
{
	m_world = DirectX::XMMatrixIdentity();
	m_projection = DirectX::XMMatrixPerspectiveFovRH(fov, aspectRatio, nearClip, farClip);
	m_position = DirectX::XMFLOAT3(0, 0, 5);
	m_rotation = DirectX::XMFLOAT3(0,0,0);
	m_scale = DirectX::XMFLOAT3(1,1,1);

	m_up = DirectX::XMFLOAT3(0, 1, 0);
	m_right = DirectX::XMFLOAT3(1, 0, 0);
	m_forward = DirectX::XMFLOAT3(0, 0, -1);
	m_zero = DirectX::XMFLOAT3(0, 0, 0);
	

	

	
	m_view = DirectX::XMMatrixLookAtRH(XMLoadFloat3(&m_position), DirectX::XMVectorAdd(XMLoadFloat3(&m_position), DirectX::XMLoadFloat3(&m_forward)), DirectX::XMLoadFloat3(&m_up));	

	m_wvp = m_world * m_view * m_projection;
	//m_wvp = XMMatrixMultiply(m_world, m_view);
	//m_wvp = XMMatrixMultiply(m_wvp, m_projection);
}

void Camera::Update()
{
	m_transform = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	m_transform = DirectX::XMMatrixMultiply(m_transform, DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z));
	m_transform = DirectX::XMMatrixMultiply(m_transform, DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z));


	
	m_view = DirectX::XMMatrixLookAtRH(XMLoadFloat3(&m_position), DirectX::XMVectorAdd(XMLoadFloat3(&m_position), DirectX::XMLoadFloat3(&m_forward)), XMLoadFloat3(&m_up));

	m_wvp = m_world * m_view * m_projection;
}