#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Camera::Camera(int width, int height)
{
	// メンバ変数の初期化
	m_eyePos = Vector3(0, 0, 5);
	m_refPos = Vector3(0, 0, 0);
	m_upVec = Vector3(0, 1, 0);
	m_fovY = XMConvertToRadians(60.0f);
	m_aspect = float(width) / float(height);
	m_nearClip = 0.1f;
	m_farClip = 1000.0f;

	// ビュー行列の生成
	m_view = Matrix::CreateLookAt(m_eyePos, m_refPos, m_upVec);

	// 射影行列の生成
	m_proj = Matrix::CreatePerspectiveFieldOfView(
			m_fovY,	// 視野角（上下方向）
			m_aspect,	// アスペクト比
			m_nearClip, // ニアクリップ
			m_farClip);	// ファークリップ
}

Camera::~Camera()
{
}

void Camera::Update()
{
	// ビュー行列の生成
	m_view = Matrix::CreateLookAt(m_eyePos, m_refPos, m_upVec);

	// 射影行列の生成
	m_proj = Matrix::CreatePerspectiveFieldOfView(
		m_fovY,	// 視野角（上下方向）
		m_aspect,	// アスペクト比
		m_nearClip, // ニアクリップ
		m_farClip);	// ファークリップ
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
