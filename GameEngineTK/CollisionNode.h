//
// �����蔻��m�[�h
//

#pragma once

#include "Obj3d.h"
#include "Collision.h"

// �����蔻��m�[�h
class CollisionNode
{
public:
	static bool GetDebugVisible() { return m_DebugVisible; }
	static void SetDebugVisible(bool visible) { m_DebugVisible = visible; }

protected:
	// �f�o�b�O�\��ON
	static bool m_DebugVisible;

public:
	// ������
	virtual void Initialize() = 0;
	// �X�V
	virtual void Update() = 0;
	// �`��
	virtual void Draw() = 0;

	void SetParent(Obj3d* parent);

	void SetTrans(const DirectX::SimpleMath::Vector3& trans) { m_Trans = trans; }

protected:
	// �f�o�b�O�\���p�I�u�W�F�N�g
	Obj3d m_Obj;
	// �e����̃I�t�Z�b�g
	DirectX::SimpleMath::Vector3 m_Trans;
};

// �������蔻��m�[�h
class SphereNode : public CollisionNode, public Sphere
{
public:
	// �R���X�g���N�^
	SphereNode();
	// ������
	void Initialize();
	// �X�V
	void Update();
	// �`��
	void Draw();

	void SetLocalRadius(float radius) { m_LocalRadius = radius; }
protected:
	// ���[�J�����a
	float m_LocalRadius;

};