//
// 自機に追従するクラス
//
#pragma once
#include "Camera.h"

class FollowCamera : public Camera
{
public:
	// コンストラクタ
	FollowCamera(int width, int height);

	// 自機とカメラの距離
	static const float CAMERA_DISTANCE;
		
	// 後進
	void Update() override;

	// 追従対象の座標をセット
	void SetTargetPos(const DirectX::SimpleMath::Vector3 targetPos);
	// 追従対象の回転角をセット
	void SetTargetAngle(float targetAngle);

protected:
	// 追従対象の座標
	DirectX::SimpleMath::Vector3 m_targetPos;
	// 追従対象の角度
	float m_targetAngle;

};
