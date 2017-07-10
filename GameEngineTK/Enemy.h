/**
*	@file	Enemy.h
*
*	@brief	敵
*
*	@date	2015/05/08
*
*	@author	藤澤　伸治
*/
#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <Keyboard.h>
#include <vector>
#include "Obj3D.h"
#include "CollisionNode.h"

// 自機
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

	// コンストラクタ
	Enemy(DirectX::Keyboard* keyboard);
	// デストラクタ
	~Enemy();
	// 初期化
	void Initialize();
	// 毎フレーム更新
	void Update();
	// 行列の計算
	void Calc();
	// 描画
	void Draw();
	// 座標を取得
	const DirectX::SimpleMath::Vector3& GetTrans();
	// 座標を取得
	const DirectX::SimpleMath::Vector3& GetRot();
	// 座標を設定
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	// 回転を設定
	void SetRot(const DirectX::SimpleMath::Vector3& rot);
	// ワールド行列を取得
	const DirectX::SimpleMath::Matrix& GetLocalWorld();
	// 弾丸用の当たり判定を取得
	const SphereNode& GetCollisionNodeBody() { return m_CollisionNodeBody; }
protected:
	// メンバ変数
	// ロボットの構成パーツ
	std::vector<Obj3d>	m_Obj;

	// キーボード
	DirectX::Keyboard* m_pKeyboard;
	DirectX::Keyboard::KeyboardStateTracker m_KeyboardTracker;

	float m_cycle;
	float m_rotate;

	// 当たり判定
	SphereNode m_CollisionNodeBody;

	// タイマー
	int m_timer;

	// 目標角度
	float m_distAngle;

	// パーツギミック用の変数
	bool m_attack;
	int m_part;
	float m_cnt;
	float m_wingRotate;

};

