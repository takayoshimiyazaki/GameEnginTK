#include "Collision.h"

using namespace DirectX::SimpleMath;


bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	// AからBに向かうベクトル
	Vector3 sub = sphereB.Center - sphereA.Center;
	// AとB距離の二乗を計算
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;

	// 半径の和の二乗を計算
	float radiusSquare = sphereA.Radius + sphereB.Radius;
	radiusSquare = radiusSquare * radiusSquare;

	// 距離の半径の和より大きければ当たっていない
	if (distanceSquare > radiusSquare)
	{
		return false;
	}

	// 当たっている
	return true;
}