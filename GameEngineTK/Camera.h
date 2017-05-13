//
// �J�����𐧌䂷��N���X
//
#pragma once

#include <d3d11.h>
#include <SimpleMath.h>

class Camera
{
public:
	// �������烁���o�֐�������
	Camera(int width,int height);
	virtual ~Camera();

	// �X�V
	void Update();

	// �r���[�s��̎擾
	DirectX::SimpleMath::Matrix GetViewMatrix();

	// �ˉe�s��̎擾
	DirectX::SimpleMath::Matrix GetProjectMatrix();

	// ���_���W�̃Z�b�g
	void SetEyePos(DirectX::SimpleMath::Vector3 eyePos);

	// �����_�̃Z�b�g
	void SetRefPos(DirectX::SimpleMath::Vector3 refPos);

	// �x�N�g���̃Z�b�g
	void SetUpVec(DirectX::SimpleMath::Vector3 upVec);

	// ����p�̃Z�b�g
	void SetFovYPos(float fovY);

	// �A�X�y�N�g��̃Z�b�g
	void SetAspectPos(float aspect);

	// �j�A�N���b�v�̃Z�b�g
	void SetNearClipPos(float nearClip);

	// �t�@�[�N���b�v�̃Z�b�g
	void SetFarClipPos(float farClip);

protected:
	// �J�����̈ʒu�i���_���W�j
	DirectX::SimpleMath::Vector3 m_eyePos;
	// �J�����̌��Ă����i�����_�j
	DirectX::SimpleMath::Vector3 m_refPos;
	// �J�����̏�����̃x�N�g��
	DirectX::SimpleMath::Vector3 m_upVec;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// ����p�i�㉺�����j
	float m_fovY;
	// �A�X�y�N�g��
	float m_aspect;
	// �j�A�N���b�v
	float m_nearClip;
	// �t�@�[�N���b�v
	float m_farClip;
	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;
};