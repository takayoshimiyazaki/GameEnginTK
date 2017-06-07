//
// 3D�I�u�W�F�N�g�̃N���X
//

#pragma once

#include <windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>
#include "Camera.h"

class Obj3d
{
public:
	// �ÓI�����o�֐�
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device>            d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>     d3dContext,
		Camera* camera);
private:
	// �ÓI�����o�ϐ�
	// �f�o�C�X
	static Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
	// �f�o�C�X�R���e�L�X�g
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
	// �ėp�X�e�[�g�ݒ�
	static std::unique_ptr<DirectX::CommonStates> m_states;
	// �G�t�F�N�g�t�@�N�g���[
	static std::unique_ptr<DirectX::EffectFactory> m_factory;
	// �J����
	static Camera* m_camera;

public:
	// �R���X�g���N�^�֐�
	Obj3d();

	// �����o�֐�

	// CMO���f����ǂݍ���
	void LoadModel(const wchar_t* fileName);
	// �X�V
	virtual void Update();
	// �`��
	void Draw();

	// setter //////////////////
	// �X�P�[�����O(XYZ)
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; }
	// ��]�p(XYZ)
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation) { m_rotation = rotation; }
	// ���s�ړ�(XYZ)
	void SetTranslation(const DirectX::SimpleMath::Vector3& translation) { m_translation = translation; }
	// �e��3D�I�u�W�F�N�g
	void SetObjParent(Obj3d* ObjParent) { m_ObjParent = ObjParent; }

	// getter ////////////////
	// �X�P�[�����O(XYZ)
	const DirectX::SimpleMath::Vector3& GetScale() { return m_scale; }
	// ��]�p(XYZ)
	const DirectX::SimpleMath::Vector3& GetRotate() { return m_rotation; }
	// ���s�ړ�(XYZ)
	const DirectX::SimpleMath::Vector3& GetTranslation() { return m_translation; }
	// ���[���h�s��
	const DirectX::SimpleMath::Matrix& GetWorld() { return m_world; }

private:
	// �����o�ϐ�

	// ���f��
	std::unique_ptr<DirectX::Model> m_model;

	// �X�P�[�����O(XYZ)
	DirectX::SimpleMath::Vector3 m_scale;
	// ��]�p(XYZ)
	DirectX::SimpleMath::Vector3 m_rotation;
	// ���s�ړ�(XYZ)
	DirectX::SimpleMath::Vector3 m_translation;

	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;

	// �e�̃I�u�W�F�N�g�ւ̃|�C���^
	Obj3d* m_ObjParent;
};
