#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera(int width, int height)
{
	// �����o�ϐ��̏�����
	m_eyePos = Vector3(0, 0, 5);
	m_refPos = Vector3(0, 0, 0);
	m_upVec = Vector3(0, 1, 0);
	m_fovY = XMConvertToRadians(60.0f);
	m_aspect = float(width) / float(height);
	m_nearClip = 0.1f;
	m_farClip = 1000.0f;

	// �r���[�s��̐���
	m_view = Matrix::CreateLookAt(m_eyePos, m_refPos, m_upVec);

	// �ˉe�s��̐���
	m_proj = Matrix::CreatePerspectiveFieldOfView(
			m_fovY,	// ����p�i�㉺�����j
			m_aspect,	// �A�X�y�N�g��
			m_nearClip, // �j�A�N���b�v
			m_farClip);	// �t�@�[�N���b�v
}

Camera::~Camera()
{
}

void Camera::Update()
{
	// �r���[�s��̐���
	m_view = Matrix::CreateLookAt(m_eyePos, m_refPos, m_upVec);

	// �ˉe�s��̐���
	m_proj = Matrix::CreatePerspectiveFieldOfView(
		m_fovY,	// ����p�i�㉺�����j
		m_aspect,	// �A�X�y�N�g��
		m_nearClip, // �j�A�N���b�v
		m_farClip);	// �t�@�[�N���b�v
}

const DirectX::SimpleMath::Matrix Camera::GetViewMatrix()
{
	return m_view;
}

const DirectX::SimpleMath::Matrix Camera::GetProjectMatrix()
{
	return m_proj;
}

void Camera::SetEyePos(const DirectX::SimpleMath::Vector3 eyePos)
{
	m_eyePos = eyePos;
}

void Camera::SetRefPos(const DirectX::SimpleMath::Vector3 refPos)
{
	m_refPos = refPos;
}

void Camera::SetUpVec(const DirectX::SimpleMath::Vector3 upVec)
{
	m_upVec = upVec;
}

void Camera::SetFovYPos(float fovY)
{
	m_fovY = fovY;
}

void Camera::SetAspectPos(float aspect)
{
	m_aspect = aspect;
}

void Camera::SetNearClipPos(float nearClip)
{
	m_nearClip = nearClip;
}

void Camera::SetFarClipPos(float farClip)
{
	m_farClip = farClip;
}
