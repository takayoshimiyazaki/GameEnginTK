//--------------------------------------------------------------------------------------
// ファイル名: Player.cpp
// 作成者:
// 作成日:
// 説明:
//--------------------------------------------------------------------------------------

#include "Player.h"

#define ROT_SPEED 10.0f

using namespace DirectX;
using namespace DirectX::SimpleMath;

Player::Player(DirectX::Keyboard* keyboard)
{
	m_pKeyboard = keyboard;

	Initialize();
}

Player::~Player()
{
}

//-----------------------------------------------------------------------------
// 初期化
//-----------------------------------------------------------------------------
void Player::Initialize()
{
	m_Obj.resize(PARTS_NUM);
	// ロボットのモデル読み込み
	m_Obj[PARTS_CATAPIRA].LoadModel(L"$Resources/catapira.cmo");
	m_Obj[PARTS_HEAD].LoadModel(L"$Resources/haed.cmo");
	m_Obj[PARTS_WING].LoadModel(L"$Resources/wing.cmo");
	m_Obj[PARTS_UFO].LoadModel(L"$Resources/ufo.cmo");

	// 親子関係の構築
	m_Obj[PARTS_HEAD].SetObjParent(&m_Obj[PARTS_CATAPIRA]);
	m_Obj[PARTS_WING].SetObjParent(&m_Obj[PARTS_HEAD]);
	m_Obj[PARTS_UFO].SetObjParent(&m_Obj[PARTS_HEAD]);
	// 子パーツから親にオフセット（座標のずれ）をセット
	m_Obj[PARTS_HEAD].SetTranslation(Vector3(0, 0.3f, 0));
	m_Obj[PARTS_WING].SetTranslation(Vector3(0, 0.3f, 0.5f));
	m_Obj[PARTS_UFO].SetTranslation(Vector3(0, 0.5f, 0));

	m_Obj[PARTS_HEAD].SetScale(Vector3(2, 2, 2));
	m_Obj[PARTS_WING].SetScale(Vector3(1, 1, 1));
	m_Obj[PARTS_UFO].SetScale(Vector3(0.5f, 0.5f, 0.5f));

	m_Obj[PARTS_WING].SetRotation(Vector3(0, 0, 0));

	m_cycle = 0.0f;

	m_attack = false;
	m_part = 0;
	m_cnt = 0;
	m_rotate = 0;
	m_wingRotate = 0;
	m_bulletCnt = 0;

	{ // 弾丸用の当たり判定ノードの設定
		m_CollisionNodeBullet.Initialize();
		// 親パーツを設定
		m_CollisionNodeBullet.SetParent(&m_Obj[PARTS_WING]);
		m_CollisionNodeBullet.SetTrans(Vector3(0, 0, -0.3f));
		m_CollisionNodeBullet.SetLocalRadius(0.5f);
	}

	{ // 全体用の当たり判定ノードの設定
		m_CollisionNodeBody.Initialize();
		m_CollisionNodeBody.SetParent(&m_Obj[PARTS_CATAPIRA]);
		m_CollisionNodeBody.SetLocalRadius(0.7f);
		m_CollisionNodeBody.SetTrans(Vector3(0, 0.7f, 0));
	}
	
	m_isJump = false;

}

//-----------------------------------------------------------------------------
// 更新
//-----------------------------------------------------------------------------
void Player::Update()
{
	Keyboard::State keystate = m_pKeyboard->GetState();
	m_KeyboardTracker.Update(keystate);

	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keys::Space))
	{
		// ジャンプ開始
		StartJump();
	}

	// 落下中であれば
	if (m_isJump)
	{
		// 重力による加速
		m_Velocity.y -= GRAVITY_ACC;

		// 下向き速度の限界処理
		if (m_Velocity.y < -JUMP_SPEED_MAX)
		{
			m_Velocity.y = -JUMP_SPEED_MAX;
		}
	}

	{// 速度による移動
		Vector3 trans = GetTrans();
		trans += m_Velocity;
		SetTrans(trans);
	}

	// パーツギミック
	{
		m_cycle += 0.1f;

		m_rotate += 0.1f;
		m_Obj[PARTS_UFO].SetRotation(Vector3(0, m_rotate, 0));

		/*if (m_attack)
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
					m_Obj[PARTS_WING].SetTranslation(Vector3(0, 1, -m_cnt));
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
					m_Obj[PARTS_WING].SetRotation(Vector3(0, 0, XMConvertToRadians(180)));
					m_Obj[PARTS_WING].SetTranslation(Vector3(0, 1, -m_cnt));
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
					m_Obj[PARTS_WING].SetRotation(Vector3(XMConvertToRadians(-90), 0, 0));
					m_Obj[PARTS_WING].SetTranslation(Vector3(0, m_cnt, 0.5f));
				}
				else
				{
					m_part = 0;
					m_cnt = 0;
					m_attack = false;
				}
				break;
			}
		}*/
	}
	//// ←回転/→回転
	//if (keystate.A)
	//{
	//	// 現在の角度を取得
	//	Quaternion rot = m_Obj[PARTS_CATAPIRA].GetRotationQ();
	//	Quaternion rotadd = Quaternion::CreateFromAxisAngle(Vector3::UnitY, ROT_SPEED);
	//	rot = rotadd * rot;
	//	// 回転後の角度を反映
	//	m_Obj[PARTS_CATAPIRA].SetRotationQ(rot);
	//}
	//if (keystate.D)
	//{
	//	// 現在の角度を取得
	//	Quaternion rot = m_Obj[PARTS_CATAPIRA].GetRotationQ();
	//	Quaternion rotadd = Quaternion::CreateFromAxisAngle(Vector3::UnitY, -ROT_SPEED);
	//	rot = rotadd * rot;
	//	// 回転後の角度を反映
	//	m_Obj[PARTS_CATAPIRA].SetRotationQ(rot);
	//}

	//// ←回転/→回転
	//if (keystate.W)
	//{
	//	// 現在の角度を取得
	//	Quaternion rot = m_Obj[PARTS_CATAPIRA].GetRotationQ();
	//	Quaternion rotadd = Quaternion::CreateFromAxisAngle(Vector3::UnitX, ROT_SPEED);
	//	rot = rotadd * rot;
	//	// 回転後の角度を反映
	//	m_Obj[PARTS_CATAPIRA].SetRotationQ(rot);
	//}
	//if (keystate.S)
	//{
	//	// 現在の角度を取得
	//	Quaternion rot = m_Obj[PARTS_CATAPIRA].GetRotationQ();
	//	Quaternion rotadd = Quaternion::CreateFromAxisAngle(Vector3::UnitX, -ROT_SPEED);
	//	rot = rotadd * rot;
	//	// 回転後の角度を反映
	//	m_Obj[PARTS_CATAPIRA].SetRotationQ(rot);
	//}

	//// 自動で前進
	//{
	//	// 現在の座標・回転角を取得
	//	Vector3 trans = m_Obj[PARTS_CATAPIRA].GetTranslation();
	//	// 移動ベクトル(Z座標前進)
	//	SimpleMath::Vector3 moveV(0, 0, -0.1f);
	//	// 移動ベクトルを回転する
	//	moveV = Vector3::TransformNormal(moveV, m_Obj[PARTS_CATAPIRA].GetWorld());
	//	// 移動
	//	trans += moveV;
	//	// 移動した座標を反映
	//	m_Obj[PARTS_CATAPIRA].SetTranslation(trans);
	//}

	// 左旋回処理
	if (keystate.A)
	{
		//回転		
		//m_headAngle += 0.03f;	
		float angle = m_Obj[0].GetRotate().y;
		m_Obj[0].SetRotation(Vector3(0, angle + 0.03f, 0));
	}
	// 右旋回処理
	if (keystate.D)
	{
		//回転		
		float angle = m_Obj[0].GetRotate().y;
		m_Obj[0].SetRotation(Vector3(0, angle - 0.03f, 0));
	}
	// 前進処理
	if (keystate.W)
	{
		// 移動ベクトル（Z軸方向移動）
		Vector3 moveV(0, 0, -0.1f);
		// 移動ベクトルを自機角度分回転させる
		//moveV = Vector3::TransformNormal(moveV, m_worldHead);
		float angle = m_Obj[0].GetRotate().y;
		Matrix rotmatY = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmatY);
		// 自機の移動する座標
		Vector3 pos = m_Obj[0].GetTranslation();
		m_Obj[0].SetTranslation(pos + moveV);
	}
	// 後進処理
	if (keystate.S)
	{
		// 移動ベクトル（Z軸方向移動）
		Vector3 moveV(0, 0, 0.1f);
		// 移動ベクトルを自機角度分回転させる
		//moveV = Vector3::TransformNormal(moveV, m_worldHead);
		float angle = m_Obj[0].GetRotate().y;
		Matrix rotmatY = Matrix::CreateRotationY(angle);
		moveV = Vector3::TransformNormal(moveV, rotmatY);
		// 自機の移動する座標
		Vector3 pos = m_Obj[0].GetTranslation();
		m_Obj[0].SetTranslation(pos + moveV);
	}

	// 弾丸パーツの前進処理
	if (m_attack == true)
	{		
		// 弾丸パーツの移動する座標
		Vector3 pos = m_Obj[PARTS_WING].GetTranslation();
		m_Obj[PARTS_WING].SetTranslation(pos + m_BulletVel);
		m_bulletCnt++;
	}
	else
	{
		// 元の位置に戻す
		m_Obj[PARTS_WING].SetObjParent(&m_Obj[PARTS_HEAD]);
		m_Obj[PARTS_WING].SetTranslation(Vector3(0, 0.3f, 0.5f));
		m_Obj[PARTS_WING].SetScale(Vector3(1, 1, 1));
		m_Obj[PARTS_WING].SetRotation(Vector3(0, 0, 0));
	}
	
	Calc();

	if (m_KeyboardTracker.IsKeyPressed(Keyboard::Keyboard::Space))
	{
		FireBullet();
		if (m_attack == false)
		{
			m_attack = true;
		}
		else
		{
			m_bulletCnt = 0;
			m_attack = false;
		}		
	}

	if (m_bulletCnt >= 120)
	{
		m_bulletCnt = 0;
		m_attack = false;
	}
	
}

////////////////////////////////////////////////////////////////
// 行列更新
///////////////////////////////////////////////////////////////
void Player::Calc()
{
	for (std::vector<Obj3d>::iterator it = m_Obj.begin(); it != m_Obj.end(); it++)
	{
		it->Update();
	}

	m_CollisionNodeBullet.Update();
	m_CollisionNodeBody.Update();
}

//-----------------------------------------------------------------------------
// 描画
//-----------------------------------------------------------------------------
void Player::Draw()
{
	// 全パーツ分描画
	for (std::vector<Obj3d>::iterator it = m_Obj.begin(); it != m_Obj.end(); it++)
	{
		it->Draw();
	}
	m_CollisionNodeBullet.Draw();
	m_CollisionNodeBody.Draw();
}

void Player::FireBullet()
{
	// 発射するパーツのワールド行列を取得
	Matrix worldm = m_Obj[PARTS_WING].GetWorld();

	// 抽出した情報をしまっておく変数
	Vector3 scale;			// ワールドスケーリング
	Quaternion rotation;	// ワールド回転
	Vector3 translation;	// ワールド座標

	// ワールド行列から各要素を抽出
	worldm.Decompose(scale, rotation, translation);

	// 親パーツから分離、独立させる
	m_Obj[PARTS_WING].SetObjParent(nullptr);

	m_Obj[PARTS_WING].SetScale(scale);
	m_Obj[PARTS_WING].SetRotationQ(rotation);
	m_Obj[PARTS_WING].SetTranslation(translation);

	// 弾丸パーツの速度を設定
	m_BulletVel = Vector3(0, 0, -0.1f);
	// パーツの向きに合わせて速度ベクトルを回転
	m_BulletVel = Vector3::Transform(m_BulletVel,rotation);
}

void Player::ResetBullet()
{

}

const DirectX::SimpleMath::Vector3& Player::GetTrans()
{
	// タンクパーツの座標を返す
	return m_Obj[0].GetTranslation();
}

const DirectX::SimpleMath::Vector3 & Player::GetRot()
{
	// タンクパーツの回転を返す
	return m_Obj[0].GetRotate();
}

void Player::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	// タンクパーツの座標を設定
	m_Obj[0].SetTranslation(trans);
}

void Player::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	// タンクパーツの座標を設定
	m_Obj[0].SetRotation(rot);
}

const DirectX::SimpleMath::Matrix& Player::GetLocalWorld()
{
	// タンクパーツのワールド行列を返す
	return m_Obj[0].GetWorld();
}

// ジャンプを開始
void Player::StartJump()
{
	if (!m_isJump)
	{
		// 上方向の初速を設定
		m_Velocity.y = JUMP_SPEED_FIRST;
		m_isJump = true;
	}
}

// 落下を開始
void Player::StartFall()
{
	if (!m_isJump)
	{
		// 上方向の初速を設定
		m_Velocity.y = 0;
		m_isJump = true;
	}
}

// 落下を終了
void Player::StopJump()
{
	m_isJump = false;
	m_Velocity = Vector3::Zero;
}