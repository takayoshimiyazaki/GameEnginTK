#include "Obj3d.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// 静的メンバ変数の定義
// デバイス
Microsoft::WRL::ComPtr<ID3D11Device>            Obj3d::m_d3dDevice;
// デバイスコンテキスト
Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;
// カメラ
Camera* Obj3d::m_camera;
// 汎用ステート設定
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;
// エフェクトファクトリー
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;

ID3D11BlendState* Obj3d::m_pBlendStateSubtract;


void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera * camera)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_camera = camera;

	// 汎用ステート設定を生成
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
	// エフェクトファクトリーを生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	// テクスチャのパスを指定
	m_factory->SetDirectory(L"$Resources");

	// 減算描画用のブレンドステートを作成
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
	// メンバ変数の初期化
	m_scale = Vector3(1, 1, 1);
	m_ObjParent = nullptr;
	// デフォルトではオイラー角を使用
	m_UseQuaternion = false;
}

//Obj3d::Obj3d(const Obj3d& obj)
//{
//	this->m_model = std::move(obj.m_model);
//}

void Obj3d::LoadModel(const wchar_t * fileName)
{
	// CMOからモデルを読み込む
	m_model = Model::CreateFromCMO(m_d3dDevice.Get(), fileName, *m_factory);
}

void Obj3d::Update()
{
	// ワールド行列を計算
	Matrix scalemat = Matrix::CreateScale(m_scale);	

	//回転
	Matrix rotmat;
	if (m_UseQuaternion)
	{	// クォータニオン回転をで計算
		rotmat = Matrix::CreateFromQuaternion(m_rotationQ);
	}
	else
	{	// オイラー角で回転を計算
		Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);
		Matrix rotmatX = Matrix::CreateRotationX(m_rotation.x);
		Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);
		rotmat = rotmatZ * rotmatX * rotmatY;
	}
	
	//平行移動
	Matrix transmat = Matrix::CreateTranslation(m_translation);
	
	// ワールド行列の合成
	m_world = scalemat * rotmat * transmat;

	// 親の行列をかける
	if (m_ObjParent)
	{
		m_world *= m_ObjParent->GetWorld();
	}	
}

void Obj3d::Draw()
{
	// 描画
	if (m_model)
	{
		m_model->Draw(m_d3dContext.Get(), *m_states, m_world, m_camera->GetViewMatrix(), m_camera->GetProjectMatrix());		
	}
}

/**
*	@brief オブジェクトのライティングを無効にする
*/
void Obj3d::DisableLighting()
{
	if (m_model)
	{
		// モデル内の全メッシュ分回す
		ModelMesh::Collection::const_iterator it_mesh = m_model->meshes.begin();
		for (; it_mesh != m_model->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);

			// メッシュ内の全メッシュパーツ分回す
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);

				// メッシュパーツにセットされたエフェクトをBasicEffectとして取得
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());
				if (eff != nullptr)
				{
					// 自己発光を最大値に
					eff->SetEmissiveColor(Vector3(1, 1, 1));

					// エフェクトに含む全ての平行光源分について処理する
					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)
					{
						// ライトを無効にする
						eff->SetLightEnabled(i, false);
					}
				}
			}
		}
	}
}

void Obj3d::SetSubtractive()
{
	// 減算描画を設定
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

		// 減算描画用の設定関数を渡して描画
		m_model->Draw(m_d3dContext.Get(), *m_states, m_world, view, projection, false, Obj3d::SetSubtractive);
	}
}
