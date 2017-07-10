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
	// ���Z�`��ݒ���Z�b�g
	static void SetSubtractive();
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

	static ID3D11BlendState* m_pBlendStateSubtract;

public:
	// �R���X�g���N�^�֐�
	Obj3d();
	// �R�s�[�R���X�g���N�^�֐�
	//Obj3d(const Obj3d& obj);

	// �����o�֐�

	// CMO���f����ǂݍ���
	void LoadModel(const wchar_t* fileName);
	// �X�V
	void Update();
	// �`��
	void Draw();
	// ���Z�`��ł̕`��i�e�p�j
	void DrawSubtractive();
	// �I�u�W�F�N�g�̃��C�e�B���O�𖳌��ɂ���
	void DisableLighting();

	// setter //////////////////
	// �X�P�[�����O(XYZ)
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; m_UseQuaternion = false; }
	// ��]�p(�I�C���[�p)
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation) { m_rotation = rotation; }
	// ��]�p(�N�H�[�^�j�I��)
	void SetRotationQ(const DirectX::SimpleMath::Quaternion& rotationQ) { m_rotationQ = rotationQ; m_UseQuaternion = true; }
	// ���s�ړ�(XYZ)
	void SetTranslation(const DirectX::SimpleMath::Vector3& translation) { m_translation = translation; }
	// �e��3D�I�u�W�F�N�g
	void SetObjParent(Obj3d* ObjParent) { m_ObjParent = ObjParent; }
	// ���[���h�s��
	void SetWorld(const DirectX::SimpleMath::Matrix& world) { m_world = world; }
	
	// getter ////////////////
	// �X�P�[�����O(XYZ)
	const DirectX::SimpleMath::Vector3& GetScale() { return m_scale; }
	// ��]�p(XYZ)
	const DirectX::SimpleMath::Vector3& GetRotate() { return m_rotation; }
	// ���s�ړ�(XYZ)
	const DirectX::SimpleMath::Vector3& GetTranslation() { return m_translation; }
	// ���[���h�s��
	const DirectX::SimpleMath::Matrix& GetWorld() { return m_world; }
	// ��]�p(�N�H�[�^�j�I��)
	const DirectX::SimpleMath::Quaternion& GetRotationQ() { return m_rotationQ; }

private:
	// �����o�ϐ�

	// ���f��
	std::unique_ptr<DirectX::Model> m_model;

	// �X�P�[�����O(XYZ)
	DirectX::SimpleMath::Vector3 m_scale;
	// ��]�p(�I�C���[�p)
	DirectX::SimpleMath::Vector3 m_rotation;
	// ��]�p(�N�H�[�^�j�I��)
	DirectX::SimpleMath::Quaternion m_rotationQ;
	// ���s�ړ�(XYZ)
	DirectX::SimpleMath::Vector3 m_translation;

	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;

	// �e�̃I�u�W�F�N�g�ւ̃|�C���^
	Obj3d* m_ObjParent;

	// ��]���N�H�[�^�j�I���ň����t���O
	bool m_UseQuaternion;
};
