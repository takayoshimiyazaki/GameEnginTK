#include "Obj3d.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// �ÓI�����o�ϐ��̒�`
// �f�o�C�X
Microsoft::WRL::ComPtr<ID3D11Device>            Obj3d::m_d3dDevice;
// �f�o�C�X�R���e�L�X�g
Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;
// �J����
Camera* Obj3d::m_camera;
// �ėp�X�e�[�g�ݒ�
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;
// �G�t�F�N�g�t�@�N�g���[
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;

ID3D11BlendState* Obj3d::m_pBlendStateSubtract;


void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera * camera)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_camera = camera;

	// �ėp�X�e�[�g�ݒ�𐶐�
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
	// �G�t�F�N�g�t�@�N�g���[�𐶐�
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	// �e�N�X�`���̃p�X���w��
	m_factory->SetDirectory(L"$Resources");

	// ���Z�`��p�̃u�����h�X�e�[�g���쐬
	D3D11_BLEND_DESC desc;
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_REV_SUBTRACT;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HRESULT ret = m_d3dDevice->CreateBlendState(&desc, &m_pBlendStateSubtract);
}

Obj3d::Obj3d()
{
	// �����o�ϐ��̏�����
	m_scale = Vector3(1, 1, 1);
	m_ObjParent = nullptr;
	// �f�t�H���g�ł̓I�C���[�p���g�p
	m_UseQuaternion = false;
}

//Obj3d::Obj3d(const Obj3d& obj)
//{
//	this->m_model = std::move(obj.m_model);
//}

void Obj3d::LoadModel(const wchar_t * fileName)
{
	// CMO���烂�f����ǂݍ���
	m_model = Model::CreateFromCMO(m_d3dDevice.Get(), fileName, *m_factory);
}

void Obj3d::Update()
{
	// ���[���h�s����v�Z
	Matrix scalemat = Matrix::CreateScale(m_scale);	

	//��]
	Matrix rotmat;
	if (m_UseQuaternion)
	{	// �N�H�[�^�j�I����]���Ōv�Z
		rotmat = Matrix::CreateFromQuaternion(m_rotationQ);
	}
	else
	{	// �I�C���[�p�ŉ�]���v�Z
		Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);
		Matrix rotmatX = Matrix::CreateRotationX(m_rotation.x);
		Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);
		rotmat = rotmatZ * rotmatX * rotmatY;
	}
	
	//���s�ړ�
	Matrix transmat = Matrix::CreateTranslation(m_translation);
	
	// ���[���h�s��̍���
	m_world = scalemat * rotmat * transmat;

	// �e�̍s���������
	if (m_ObjParent)
	{
		m_world *= m_ObjParent->GetWorld();
	}	
}

void Obj3d::Draw()
{
	// �`��
	if (m_model)
	{
		m_model->Draw(m_d3dContext.Get(), *m_states, m_world, m_camera->GetViewMatrix(), m_camera->GetProjectMatrix());		
	}
}

/**
*	@brief �I�u�W�F�N�g�̃��C�e�B���O�𖳌��ɂ���
*/
void Obj3d::DisableLighting()
{
	if (m_model)
	{
		// ���f�����̑S���b�V������
		ModelMesh::Collection::const_iterator it_mesh = m_model->meshes.begin();
		for (; it_mesh != m_model->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// ���b�V�����̑S���b�V���p�[�c����
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// ���b�V���p�[�c�ɃZ�b�g���ꂽ�G�t�F�N�g��BasicEffect�Ƃ��Ď擾
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());
				if (eff != nullptr)
				{
					// ���Ȕ������ő�l��
					eff->SetEmissiveColor(Vector3(1, 1, 1));

					// �G�t�F�N�g�Ɋ܂ޑS�Ă̕��s�������ɂ��ď�������
					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)
					{
						// ���C�g�𖳌��ɂ���
						eff->SetLightEnabled(i, false);
					}
				}
			}
		}
	}
}

void Obj3d::SetSubtractive()
{
	// ���Z�`���ݒ�
	m_d3dContext->OMSetBlendState(m_pBlendStateSubtract, nullptr, 0xffffff);
}

void Obj3d::DrawSubtractive()
{
	if (m_model)
	{
		assert(m_camera);
		const Matrix& view = m_camera->GetViewMatrix();
		const Matrix& projection = m_camera->GetProjectMatrix();

		assert(m_d3dContext);
		assert(m_states);

		// ���Z�`��p�̐ݒ�֐���n���ĕ`��
		m_model->Draw(m_d3dContext.Get(), *m_states, m_world, view, projection, false, Obj3d::SetSubtractive);
	}
}
