//--------------------------------------------------------------------------------------
// �t�@�C����: Enemy.cpp
// �쐬��:
// �쐬��:
// ����:
//--------------------------------------------------------------------------------------

#include "Enemy.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Enemy::Enemy(DirectX::Keyboard* keyboard)
: m_cycle(0.0f)
{
	m_pKeyboard = keyboard;

	Initialize();
}

Enemy::~Enemy()
{
}

//-----------------------------------------------------------------------------
// ������
//-----------------------------------------------------------------------------
void Enemy::Initialize()
{
	m_Obj.resize(PARTS_NUM);
	// ���{�b�g�̃��f���ǂݍ���
	m_Obj[ PARTS_CATAPIRA].LoadModel(L"$Resources/catapira.cmo");
	m_Obj[ PARTS_HEAD].LoadModel(L"$Resources/haed.cmo");
	m_Obj[ PARTS_WING].LoadModel(L"$Resources/wing.cmo");
	m_Obj[ PARTS_UFO].LoadModel(L"$Resources/ufo.cmo");

	// �e�q�֌W�̍\�z
	m_Obj[ PARTS_HEAD].SetObjParent(&m_Obj[ PARTS_CATAPIRA]);
	m_Obj[ PARTS_WING].SetObjParent(&m_Obj[ PARTS_HEAD]);
	m_Obj[ PARTS_UFO].SetObjParent(&m_Obj[ PARTS_HEAD]);

	// �q�p�[�c����e�ɃI�t�Z�b�g�i���W�̂���j���Z�b�g
	m_Obj[ PARTS_HEAD].SetTranslation(Vector3(0, 0.3f, 0));
	m_Obj[ PARTS_WING].SetTranslation(Vector3(0, 0.3f, 0.5f));
	m_Obj[ PARTS_UFO].SetTranslation(Vector3(0, 0.5f, 0));

	m_Obj[ PARTS_HEAD].SetScale(Vector3(2, 2, 2));
	m_Obj[ PARTS_WING].SetScale(Vector3(1, 1, 1));
	m_Obj[ PARTS_UFO].SetScale(Vector3(0.5f, 0.5f, 0.5f));

	m_Obj[ PARTS_WING].SetRotation(Vector3(XMConvertToRadians(-90), 0, 0));

	m_cycle = 0.0f;

	// �����ʒu������
	Vector3 pos;

	pos.x = rand() % 10;
	pos.z = rand() % 10;

	SetTrans(pos);

	{ // �S�̗p�̓����蔻��m�[�h�̐ݒ�
		m_CollisionNodeBody.Initialize();
		m_CollisionNodeBody.SetParent(&m_Obj[PARTS_CATAPIRA]);
		m_CollisionNodeBody.SetLocalRadius(0.7f);
		m_CollisionNodeBody.SetTrans(Vector3(0, 0.7f, 0));
	}

	m_attack = false;
	m_part = 0;
	m_cnt = 0;
	m_rotate = 0;
	m_wingRotate = 0;

	m_timer = 0;
	m_distAngle = 0;

}

//-----------------------------------------------------------------------------
// �X�V
//-----------------------------------------------------------------------------
void Enemy::Update()
{
	Keyboard::State keystate = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keystate);

	// �p�[�c�M�~�b�N
	/*{
		m_cycle += 0.1f;

		m_rotate += 0.1f;
		m_Obj[ PARTS_UFO].SetRotation(Vector3(0, m_rotate, 0));

		if (m_attack)
		{
			switch (m_part)
			{
			case 0:
				if (m_cnt <= 1)
				{
					m_cnt += 0.1f;
					m_Obj[PARTS_WING].SetTranslation(Vector3(0, m_cnt, 0));
					m_Obj[PARTS_WING].SetRotation(Vector3(XMConvertToRadians(-180), 0, 0));
				}
				else
				{
					m_part = 1;
					m_cnt = 0;
				}
				break;
			case 1:
				if (m_cnt <= 10)
				{
					m_cnt += 0.1f;
					m_Obj[ PARTS_WING].SetTranslation(Vector3(0, 1, -m_cnt));
				}
				else
				{
					m_part = 2;
				}
				break;
			case 2:
				if (m_cnt > 0)
				{
					m_cnt -= 0.1f;
					m_Obj[ PARTS_WING].SetRotation(Vector3(0, 0, XMConvertToRadians(180)));
					m_Obj[ PARTS_WING].SetTranslation(Vector3(0, 1, -m_cnt));
				}
				else
				{
					m_part = 3;
					m_cnt = 1;
				}
				break;
			case 3:
				if (m_cnt > 0.3f)
				{
					m_cnt -= 0.1f;
					m_Obj[ PARTS_WING].SetRotation(Vector3(XMConvertToRadians(-90), 0, 0));
					m_Obj[ PARTS_WING].SetTranslation(Vector3(0, m_cnt, 0.5f));
				}
				else
				{
					m_part = 0;
					m_cnt = 0;
					m_attack = false;
				}
				break;
			}
		}
	}*/

	m_timer--;
	if (m_timer < 0)
	{
		m_timer = 60;

		// �ڕW�p�������_���Œ��I
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		// -90�`+90�̗���
		rnd += 180.0f;
		rnd = XMConvertToRadians(rnd);

		m_distAngle += rnd;
	}

	// �����񏈗�
	{
		//��]		
		Vector3 rot = GetRot();

		// ���̊p�x�ƖڕW�p�x�̍�
		float angle = m_distAngle - rot.y;

		if (angle > XM_PI)
		{
			angle -= XM_2PI;
		}
		if (angle < -XM_PI)
		{
			angle += XM_2PI;
		}

		// ���
		rot.y += angle * 0.01f;

		SetRot(rot);
	}
	
	// �@�̂������Ă�������ɑO�i����
	{
		// �ړ��x�N�g���iZ�������ړ��j
		Vector3 moveV(0, 0, -0.1f);
		// �ړ��x�N�g�������@�p�x����]������
		//moveV = Vector3::TransformNormal(moveV, m_worldHead);
		float angle = m_Obj[0].GetRotate().y;
		Matrix rotmatY = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmatY);
		// ���@�̈ړ�������W
		Vector3 pos = m_Obj[0].GetTranslation();
		m_Obj[0].SetTranslation(pos + moveV);
	}

	Calc();
	
	
}

////////////////////////////////////////////////////////////////
// �s��X�V
///////////////////////////////////////////////////////////////
void Enemy::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin(); it != m_Obj.end(); it++)
	{
		it->Update();

	}
	m_CollisionNodeBody.Update();
}

//-----------------------------------------------------------------------------
// �`��
//-----------------------------------------------------------------------------
void Enemy::Draw()
{
	// �S�p�[�c���`��
	for (std::vector<Obj3d>::iterator it = m_Obj.begin(); it != m_Obj.end(); it++)
	{
		it->Draw();

	}
	m_CollisionNodeBody.Draw();
}

const DirectX::SimpleMath::Vector3& Enemy::GetTrans()
{
	// �^���N�p�[�c�̍��W��Ԃ�
	return m_Obj[PARTS_CATAPIRA].GetTranslation();
}

const DirectX::SimpleMath::Vector3 & Enemy::GetRot()
{
	// �^���N�p�[�c�̉�]��Ԃ�
	return m_Obj[PARTS_CATAPIRA].GetRotate();
}

void Enemy::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// �^���N�p�[�c�̍��W��ݒ�
	m_Obj[PARTS_CATAPIRA].SetTranslation(trans);
}

void Enemy::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	// �^���N�p�[�c�̍��W��ݒ�
	m_Obj[PARTS_CATAPIRA].SetRotation(rot);
}

const DirectX::SimpleMath::Matrix& Enemy::GetLocalWorld()
{
	// �^���N�p�[�c�̃��[���h�s���Ԃ�
	return m_Obj[PARTS_CATAPIRA].GetWorld();
}

