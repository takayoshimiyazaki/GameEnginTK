//
// ���@�ɒǏ]����N���X
//
#pragma once
#include "Camera.h"

class FollowCamera : public Camera
{
public:
	// �R���X�g���N�^
	FollowCamera(int width, int height);

	// ���@�ƃJ�����̋���
	static const float CAMERA_DISTANCE;
		
	// ��i
	void Update() override;

	// �Ǐ]�Ώۂ̍��W���Z�b�g
	void SetTargetPos(const DirectX::SimpleMath::Vector3 targetPos);
	// �Ǐ]�Ώۂ̉�]�p���Z�b�g
	void SetTargetAngle(float targetAngle);

protected:
	// �Ǐ]�Ώۂ̍��W
	DirectX::SimpleMath::Vector3 m_targetPos;
	// �Ǐ]�Ώۂ̊p�x
	float m_targetAngle;

};
