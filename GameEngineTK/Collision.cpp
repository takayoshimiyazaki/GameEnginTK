#include "Collision.h"

using namespace DirectX::SimpleMath;


bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	// A����B�Ɍ������x�N�g��
	Vector3 sub = sphereB.Center - sphereA.Center;
	// A��B�����̓����v�Z
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;

	// ���a�̘a�̓����v�Z
	float radiusSquare = sphereA.Radius + sphereB.Radius;
	radiusSquare = radiusSquare * radiusSquare;

	// �����̔��a�̘a���傫����Γ������Ă��Ȃ�
	if (distanceSquare > radiusSquare)
	{
		return false;
	}

	// �������Ă���
	return true;
}