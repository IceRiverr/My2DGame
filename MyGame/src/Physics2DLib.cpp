
#include "Physics2DLib.h"

bool Collide2D(float x, float y, const AABB& aabb)
{
	if (x > aabb.LT.x && x < aabb.RB.x &&
		y > aabb.RB.y && y < aabb.LT.y)
		return true;
	else
		return false;
}

bool Collide2D(const AABB& c1, const AABB& c2)
{
	if (Collide2D(c1.LT.x, c1.LT.y, c2))
		return true;
	if (Collide2D(c1.RB.x,c1.RB.y, c2))
		return true;
	if (Collide2D(c1.RB.x, c1.LT.y, c2))
		return true;
	if (Collide2D(c1.LT.x, c1.RB.y, c2))
		return true;
	return false;
}
