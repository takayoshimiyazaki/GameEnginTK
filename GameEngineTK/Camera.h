//
// カメラを制御するクラス
//
#pragma once

#include <d3d11.h>
#include <SimpleMath.h>

class Camera
{
public:
	// ここからメンバ関数を書く
	Camera(int width,int height);
	virtual ~Camera();

	// 更新
	void Update();

	// ビュー行列の取得
	DirectX::SimpleMath::Matrix GetViewMatrix();

	// 射影行列の取得
	DirectX::SimpleMath::Matrix GetProjectMatrix();

	// 視点座標のセット
	void SetEyePos(DirectX::SimpleMath::Vector3 eyePos);

	// 注視点のセット
	void SetRefPos(DirectX::SimpleMath::Vector3 refPos);

	// ベクトルのセット
	void SetUpVec(DirectX::SimpleMath::Vector3 upVec);

	// 視野角のセット
	void SetFovYPos(float fovY);

	// アスペクト比のセット
	void SetAspectPos(float aspect);

	// ニアクリップのセット
	void SetNearClipPos(float nearClip);

	// ファークリップのセット
	void SetFarClipPos(float farClip);

protected:
	// カメラの位置（視点座標）
	DirectX::SimpleMath::Vector3 m_eyePos;
	// カメラの見ている先（注視点）
	DirectX::SimpleMath::Vector3 m_refPos;
	// カメラの上方向のベクトル
	DirectX::SimpleMath::Vector3 m_upVec;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// 視野角（上下方向）
	float m_fovY;
	// アスペクト比
	float m_aspect;
	// ニアクリップ
	float m_nearClip;
	// ファークリップ
	float m_farClip;
	// 射影行列
	DirectX::SimpleMath::Matrix m_proj;
};