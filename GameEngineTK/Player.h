/**
*	@file	Player.h
*
*	@brief	���@
*
*	@date	2017/06/08
*
*	@author	�{��M�P
*/
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>
#include "Obj3D.h"
#include "CollisionNode.h"

// ���@
class Player
{
public:
	// �d�͉����x
	const float GRAVITY_ACC = 0.03f;
	// �W�����v�����x
	const float JUMP_SPEED_FIRST = 0.5f;
	// �W�����v���x���� 
	const float JUMP_SPEED_MAX = 0.3f;

	// ���@�p�[�c
	enum PARTS
	{
		PARTS_CATAPIRA,
		PARTS_HEAD,
		PARTS_WING,
		PARTS_UFO,

		PARTS_NUM
	};

	// �R���X�g���N�^
	Player(DirectX::Keyboard* keyboard);
	// �f�X�g���N�^
	~Player();
	// ������
	void Initialize();
	// ���t���[���X�V
	void Update();
	// �s��̌v�Z
	void Calc();
	// �`��
	void Draw();
	// �e�ۂ𔭎�
	void FireBullet();
	// �e�ۂ��đ��U�i���Z�b�g�j
	void ResetBullet();
	// ���W���擾
	const DirectX::SimpleMath::Vector3& GetTrans();
	// ���W���擾
	const DirectX::SimpleMath::Vector3& GetRot();
	// ���W��ݒ�
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	// ��]��ݒ�
	void SetRot(const DirectX::SimpleMath::Vector3& rot);
	// ���[���h�s����擾
	const DirectX::SimpleMath::Matrix& GetLocalWorld();
	void StartJump();
	void StartFall();
	void StopJump();
	// �e�ۗp�̓����蔻����擾
	const SphereNode& GetCollisionNodeBullet() { return m_CollisionNodeBullet; }
	// �S�̗p�̓����蔻����擾
	const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }
	// ���x���擾
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_Velocity; }

protected:
	// �����o�ϐ�
	// ���{�b�g�̍\���p�[�c
	std::vector<Obj3d> m_Obj;

	// �L�[�{�[�h
	DirectX::Keyboard* m_pKeyboard;
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

	float m_cycle;
	float m_rotate;

	// �p�[�c�M�~�b�N�p�̕ϐ�
	bool m_attack;
	int m_part;
	float m_cnt;
	float m_wingRotate;

	// �e�ۂ̑��x�x�N�g��
	DirectX::SimpleMath::Vector3 m_BulletVel;

	// �o���b�g�̎���
	int m_bulletCnt;

	// �e�ۗp�̓����蔻��
	SphereNode m_CollisionNodeBullet;

	// �����蔻��
	SphereNode m_CollisionNodeBody;
	// ���x
	DirectX::SimpleMath::Vector3 m_Velocity;
	// �������t���O
	bool m_isJump;
};