#include "FollowCamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

const float FollowCamera::CAMERA_DISTANCE = 5.0f;

FollowCamera::FollowCamera(int width, int height)
	:Camera(width,height)
{
	m_targetPos = Vector3(0.0f, 0.0f, 0.0f);
	m_targetAngle = 0.0f;
	m_keyboard = nullptr;
	m_isFPS = true;
}

void FollowCamera::Update()
{
	Keyboard::State keyState = m_keyboard->GetState();
	m_keyboardTracker.Update(keyState);
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keyboard::C))
	{
		// カメラの切り替え
		m_isFPS = !m_isFPS;
	}

	Vector3 eyePos, refPos;
	if (m_isFPS)
	{
		// FPSカメラ
		{
			Vector3 position;
			// カメラ座標を計算
			position = m_targetPos + Vector3(0, 0.2f, 0);
			Vector3 cameraV(0.0f, 0.0f, -CAMERA_DISTANCE);
			Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
			cameraV = Vector3::TransformNormal(cameraV, rotmat);
			eyePos = position + cameraV * 0.1f;
			refPos = position + cameraV;
		}
	}
	else
	{
		// TPSカメラ
		{
			refPos = m_targetPos + Vector3(0.0f, 2.0f, 0.0f);
			Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);
			Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
			cameraV = Vector3::TransformNormal(cameraV, rotmat);
			//カメラ座標を計算
			eyePos = refPos + cameraV;

			// 視点を現在位置から補間する
			eyePos = m_eyePos + (eyePos - m_eyePos) * 0.05f;
			// 参照点を現在位置から補間する
			refPos = m_refPos + (refPos - m_refPos) * 0.20f;
		}
	}	
	

	SetEyePos(eyePos);
	SetRefPos(refPos);

	// 基底クラスの更新
	Camera::Update();
}

void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3& targetPos)
{
	m_targetPos = targetPos;
}

void FollowCamera::SetTargetAngle(float targetAngle)
{
	m_targetAngle = targetAngle;
}

void FollowCamera::SetKeyboard(DirectX::Keyboard * keyboard)
{
	m_keyboard = keyboard;
}
