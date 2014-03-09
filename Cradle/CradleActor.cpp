#include "CradleActor.h"

CradleActor::CradleActor(CradleMesh* mesh)
{
	m_mesh = mesh;
	DirectX::XMStoreFloat4x4(&m_mWorld, DirectX::XMMatrixIdentity());
	m_vPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vScale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Offset = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

CradleActor::CradleActor(void)
{
	m_mesh = nullptr;
	DirectX::XMStoreFloat4x4(&m_mWorld, DirectX::XMMatrixIdentity());
	m_vPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vScale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_Offset = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);


}


CradleActor::~CradleActor(void)
{
	m_mesh = nullptr;
}

void CradleActor::Update(BasicTimer* time)
{
	m_vRotation.x += m_rotationRate * time->GetDelta();
	m_vRotation.y += m_rotationRate * 0.66f * time->GetDelta();
	// m_vPosition.x = 0.0f + sin((1.0f) * time->GetTotal() * m_rotationRate) * 10.0f;

	m_vScale.x = m_rotationRate;
	m_vScale.y = m_rotationRate;
	m_vScale.z = m_rotationRate;


	
	DirectX::XMMATRIX world = DirectX::XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z)

		*DirectX::XMMatrixRotationRollPitchYaw(m_vRotation.x, m_vRotation.y, m_vRotation.z)
		*DirectX::XMMatrixTranslation(m_vPosition.x + m_Offset.x, m_vPosition.y + m_Offset.y, m_vPosition.z + m_Offset.z);
		// * DirectX::XMMatrixTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z );


	

		
	

	DirectX::XMStoreFloat4x4(&m_mWorld, /* */ DirectX::XMMatrixTranspose(world) /* */ /* world */);
}

void CradleActor::Draw()
{

	if (m_mesh)
	{
		m_mesh->Draw(&m_mWorld);
	}
}

void CradleActor::Initialize()
{

}

void CradleActor::SetMesh(CradleMesh* mesh)
{
	m_mesh = mesh;
}