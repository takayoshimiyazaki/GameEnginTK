//
// 3Dオブジェクトのクラス
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
	// 静的メンバ関数
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device>            d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>     d3dContext,
		Camera* camera);
	// 減算描画設定をセット
	static void SetSubtractive();
private:
	// 静的メンバ変数
	// デバイス
	static Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
	// デバイスコンテキスト
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
	// 汎用ステート設定
	static std::unique_ptr<DirectX::CommonStates> m_states;
	// エフェクトファクトリー
	static std::unique_ptr<DirectX::EffectFactory> m_factory;
	// カメラ
	static Camera* m_camera;	

	static ID3D11BlendState* m_pBlendStateSubtract;

public:
	// コンストラクタ関数
	Obj3d();
	// コピーコンストラクタ関数
	//Obj3d(const Obj3d& obj);

	// メンバ関数

	// CMOモデルを読み込む
	void LoadModel(const wchar_t* fileName);
	// 更新
	void Update();
	// 描画
	void Draw();
	// 減算描画での描画（影用）
	void DrawSubtractive();
	// オブジェクトのライティングを無効にする
	void DisableLighting();

	// setter //////////////////
	// スケーリング(XYZ)
	void SetScale(const DirectX::SimpleMath::Vector3& scale) { m_scale = scale; m_UseQuaternion = false; }
	// 回転角(オイラー角)
	void SetRotation(const DirectX::SimpleMath::Vector3& rotation) { m_rotation = rotation; }
	// 回転角(クォータニオン)
	void SetRotationQ(const DirectX::SimpleMath::Quaternion& rotationQ) { m_rotationQ = rotationQ; m_UseQuaternion = true; }
	// 平行移動(XYZ)
	void SetTranslation(const DirectX::SimpleMath::Vector3& translation) { m_translation = translation; }
	// 親の3Dオブジェクト
	void SetObjParent(Obj3d* ObjParent) { m_ObjParent = ObjParent; }
	// ワールド行列
	void SetWorld(const DirectX::SimpleMath::Matrix& world) { m_world = world; }
	
	// getter ////////////////
	// スケーリング(XYZ)
	const DirectX::SimpleMath::Vector3& GetScale() { return m_scale; }
	// 回転角(XYZ)
	const DirectX::SimpleMath::Vector3& GetRotate() { return m_rotation; }
	// 平行移動(XYZ)
	const DirectX::SimpleMath::Vector3& GetTranslation() { return m_translation; }
	// ワールド行列
	const DirectX::SimpleMath::Matrix& GetWorld() { return m_world; }
	// 回転角(クォータニオン)
	const DirectX::SimpleMath::Quaternion& GetRotationQ() { return m_rotationQ; }

private:
	// メンバ変数

	// モデル
	std::unique_ptr<DirectX::Model> m_model;

	// スケーリング(XYZ)
	DirectX::SimpleMath::Vector3 m_scale;
	// 回転角(オイラー角)
	DirectX::SimpleMath::Vector3 m_rotation;
	// 回転角(クォータニオン)
	DirectX::SimpleMath::Quaternion m_rotationQ;
	// 平行移動(XYZ)
	DirectX::SimpleMath::Vector3 m_translation;

	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;

	// 親のオブジェクトへのポインタ
	Obj3d* m_ObjParent;

	// 回転をクォータニオンで扱うフラグ
	bool m_UseQuaternion;
};
