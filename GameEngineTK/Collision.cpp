#include "Collision.h"

using namespace DirectX::SimpleMath;


bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	// A‚©‚çB‚ÉŒü‚©‚¤ƒxƒNƒgƒ‹
	Vector3 sub = sphereB.Center - sphereA.Center;
	// A‚ÆB‹——£‚Ì“ñæ‚ðŒvŽZ
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;

	// ”¼Œa‚Ì˜a‚Ì“ñæ‚ðŒvŽZ
	float radiusSquare = sphereA.Radius + sphereB.Radius;
	radiusSquare = radiusSquare * radiusSquare;

	// ‹——£‚Ì”¼Œa‚Ì˜a‚æ‚è‘å‚«‚¯‚ê‚Î“–‚½‚Á‚Ä‚¢‚È‚¢
	if (distanceSquare > radiusSquare)
	{
		return false;
	}

	// “–‚½‚Á‚Ä‚¢‚é
	return true;
}