#include "CradleCamera.h"
#include "CradleGame.h"
#include "CradleD3D.h"
#include "CradleInput.h"

using namespace DirectX;

CradleCamera::CradleCamera(CradleGame* game)
	: m_vPosition(0.0f, 0.0f, 0.0f)
	, m_vRotation(0.0f, 0.0f, 0.0f)
	, m_vScale(1.0f, 1.0f, 1.0f)
	, m_vForward(0.0f, 0.0f, 1.0f)
	, m_vUp(0.0f, 1.0f, 0.0f)
	, m_vRight(1.0f, 0.0f, 0.0f)
	, FORWARD(0.0f, 0.0f, 1.0f)
	, UP(0.0f, 1.0f, 0.0f)
	, RIGHT(1.0f, 0.0f, 0.0f)
{
	m_game = game;
	XMStoreFloat4x4(&m_mWorld, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_mView, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_mProjection, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_mWVP, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_mTransform, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_mRotation, DirectX::XMMatrixIdentity());
}

void CradleCamera::Initialize(float FOV, float aspectRatio, float nearClip, float farClip)
{
	float ar = 1280.0f / 720.0f;

	wprintf_s(L"aspectRatio: %f, ar: %f\n", aspectRatio, ar);

	XMStoreFloat4x4(&m_mProjection, (DirectX::XMMatrixPerspectiveFovLH(FOV, aspectRatio, nearClip, farClip)));



	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(cbTransform);
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr = m_game->m_cd3d->m_d3dDevice->CreateBuffer(&desc, 0, &m_constantBuffer);
	HRPRINT(hr, L"Failed to create constant buffer\n", L"Created constant buffer\n");
}


#pragma unmanaged
CradleCamera::~CradleCamera()
{
	ReleaseCOM(m_constantBuffer);

}

#pragma unmanaged
void CradleCamera::Update()
{
	DirectX::XMVECTOR v;
	DirectX::XMVECTOR v2;
	DirectX::XMVECTOR v3;
	DirectX::XMMATRIX m;
	DirectX::XMMATRIX m2;
	DirectX::XMMATRIX m3;
	// Get input, update vectors
		// Updates rotation & position
	float dt = m_game->m_timer->GetDelta();

	v = DirectX::XMLoadFloat3(&m_vRotation);
	DirectX::XMStoreFloat4x4(&m_mRotation, DirectX::XMMatrixRotationRollPitchYawFromVector(v));
	
	m = DirectX::XMLoadFloat4x4(&m_mRotation);

	v = DirectX::XMLoadFloat3(&FORWARD);
	v = DirectX::XMVector3Transform(v, m);
	DirectX::XMStoreFloat3(&m_vForward, v);
	
	v = DirectX::XMLoadFloat3(&UP);
	v = DirectX::XMVector3Transform(v, m);
	DirectX::XMStoreFloat3(&m_vUp, v);

	v = DirectX::XMLoadFloat3(&RIGHT);
	v = DirectX::XMVector3Transform(v, m);
	DirectX::XMStoreFloat3(&m_vRight, v);




	UpdateControls();



	v = DirectX::XMLoadFloat3(&m_vPosition);
	v2 = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&m_vForward));

	
	DirectX::XMVECTOR newpos = DirectX::XMVectorAdd(v, v2);
	v3 = DirectX::XMLoadFloat3(&m_vUp);

	// newpos = DirectX::XMVectorZero();


	m2 = DirectX::XMMatrixLookAtLH(v, newpos, v3);


	m = DirectX::XMMatrixIdentity();
	
	DirectX::XMStoreFloat4x4(&m_mView, m2);
	DirectX::XMStoreFloat4x4(&bufferObject.View, DirectX::XMMatrixTranspose(m2));



	m3 = DirectX::XMLoadFloat4x4(&m_mProjection);
	DirectX::XMStoreFloat4x4(&bufferObject.Projection, DirectX::XMMatrixTranspose(m3));


	DirectX::XMMATRIX vp = m2 * m3;

	DirectX::XMStoreFloat4x4(&bufferObject.ViewProj, DirectX::XMMatrixTranspose(vp));

	
	DirectX::XMMATRIX wvp = DirectX::XMMatrixTranspose(m * m2 * m3);
	DirectX::XMStoreFloat4x4(&m_mWVP, wvp);


	// m_mWVP = DirectX::XMMatrixTranspose(m_mWVP);
	bufferObject.WVP = m_mWVP;

	// DirectX::XMStoreFloat4x4(&bufferObject.View, DirectX::XMMatrixTranspose(m2));

	// DirectX::XMMATRIX proj = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_mProjection));
	

	// DirectX::XMStoreFloat4x4(&bufferObject.Projection, proj);

	m_game->m_cd3d->m_d3dContext->UpdateSubresource(m_constantBuffer, 0, 0, &bufferObject, 0, 0);
	m_game->m_cd3d->m_d3dContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);


}

void CradleCamera::UpdateControls()
{
	DirectX::XMVECTOR v;
	DirectX::XMVECTOR v2;

	float dt = m_game->m_timer->GetDelta();

	if (m_game->m_cInput->KeyDown(DIK_W))
	{
		v = DirectX::XMLoadFloat3(&m_vPosition);
		v2 = DirectX::XMLoadFloat3(&m_vForward);
		v2 = v2 * dt * m_Speed;
		v += v2;
		DirectX::XMStoreFloat3(&m_vPosition, v);
	}
	else if (m_game->m_cInput->KeyDown(DIK_S))
	{
		v = DirectX::XMLoadFloat3(&m_vPosition);
		v2 = DirectX::XMLoadFloat3(&m_vForward);
		v2 = v2 * dt * -m_Speed;
		v += v2;
		DirectX::XMStoreFloat3(&m_vPosition, v);
	}

	if (m_game->m_cInput->KeyDown(DIK_D))
	{
		v = DirectX::XMLoadFloat3(&m_vPosition);
		v2 = DirectX::XMLoadFloat3(&m_vRight);
		v2 = v2 * dt * m_Speed;
		v += v2;
		DirectX::XMStoreFloat3(&m_vPosition, v);
	}
	else if (m_game->m_cInput->KeyDown(DIK_A))
	{
		v = DirectX::XMLoadFloat3(&m_vPosition);
		v2 = DirectX::XMLoadFloat3(&m_vRight);
		v2 = v2 * dt * -m_Speed;
		v += v2;
		DirectX::XMStoreFloat3(&m_vPosition, v);
	}

	if (m_game->m_cInput->KeyDown(DIK_R))
	{
		v = DirectX::XMLoadFloat3(&m_vPosition);
		v2 = DirectX::XMLoadFloat3(&m_vUp);
		v2 = v2 * dt * m_Speed;
		v += v2;
		DirectX::XMStoreFloat3(&m_vPosition, v);
	}
	else if (m_game->m_cInput->KeyDown(DIK_F))
	{
		v = DirectX::XMLoadFloat3(&m_vPosition);
		v2 = DirectX::XMLoadFloat3(&m_vUp);
		v2 = v2 * dt * -m_Speed;
		v += v2;
		DirectX::XMStoreFloat3(&m_vPosition, v);
	}

	if (m_game->m_cInput->KeyDown(DIK_Q))
		m_vRotation.y -= 0.5f * dt;
	else if (m_game->m_cInput->KeyDown(DIK_E))
		m_vRotation.y += 0.5f * dt;

	if (m_game->m_cInput->KeyDown(DIK_T))
		m_vRotation.x -= 0.5f * dt;
	else if (m_game->m_cInput->KeyDown(DIK_G))
		m_vRotation.x += 0.5f * dt;
}