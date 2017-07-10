/**
*	@file	Enemy.h
*
*	@brief	�G
*
*	@date	2015/05/08
*
*	@author	���V�@�L��
*/
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>
#include "Obj3D.h"
#include "CollisionNode.h"

// ���@
class Enemy
{
public:

	enum PARTS
	{
		PARTS_CATAPIRA,
		PARTS_HEAD,
		PARTS_WING,
		PARTS_UFO,

		PARTS_NUM
	};

	// �R���X�g���N�^
	Enemy(DirectX::Keyboard* keyboard);
	// �f�X�g���N�^
	~Enemy();
	// ������
	void Initialize();
	// ���t���[���X�V
	void Update();
	// �s��̌v�Z
	void Calc();
	// �`��
	void Draw();
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
	// �e�ۗp�̓����蔻����擾
	const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }
protected:
	// �����o�ϐ�
	// ���{�b�g�̍\���p�[�c
	std::vector<Obj3d>	m_Obj;

	// �L�[�{�[�h
	DirectX::Keyboard* m_pKeyboard;
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

	float m_cycle;
	float m_rotate;

	// �����蔻��
	SphereNode m_CollisionNodeBody;

	// �^�C�}�[
	int m_timer;

	// �ڕW�p�x
	float m_distAngle;

	// �p�[�c�M�~�b�N�p�̕ϐ�
	bool m_attack;
	int m_part;
	float m_cnt;
	float m_wingRotate;

};

