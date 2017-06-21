
#include "Physics2DLib.h"

bool Collide2D(float x, float y, const BBox& bb)
{
	if (x > bb.pos.x - bb.size.x * 0.5f && x < bb.pos.x + bb.size.x * 0.5f &&
		y > bb.pos.y - bb.size.y * 0.5f && y < bb.pos.y + bb.size.y * 0.5f)
		return true;
	else
		return false;
}

bool Collide2D(const BBox& b1, const BBox& b2)
{
	if (Collide2D(b1.pos.x - b1.size.x * 0.5f, b1.pos.y - b1.size.y * 0.5f, b2))
		return true;
	if (Collide2D(b1.pos.x - b1.size.x * 0.5f, b1.pos.y + b1.size.y * 0.5f, b2))
		return true;
	if (Collide2D(b1.pos.x + b1.size.x * 0.5f, b1.pos.y + b1.size.y * 0.5f, b2))
		return true;
	if (Collide2D(b1.pos.x + b1.size.x * 0.5f, b1.pos.y - b1.size.y * 0.5f, b2))
		return true;
	return false;
}

// 
bool LineRectCrossTest(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2)
{
	bool ret = (
		glm::min(p1.x, p2.x) <= glm::max(q1.x, q2.x) &&
		glm::min(q1.x, q2.x) <= glm::max(p1.x, p2.x) &&
		glm::min(p1.y, p2.y) <= glm::max(q1.y, q2.y) &&
		glm::min(q1.y, q2.y) <= glm::max(p1.y, p2.y));
	return ret;
}

bool LineSegmentCrossTest(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2)
{
	glm::vec3 q1q2 = glm::vec3(q2 - q1, 0.0f);
	glm::vec3 q1p1 = glm::vec3(p1 - q1, 0.0f);
	glm::vec3 q1p2 = glm::vec3(p2 - q1, 0.0f);

	float t1 = glm::dot(glm::cross(q1q2, q1p1), glm::cross(q1q2, q1p2));

	glm::vec3 p1p2 = glm::vec3(p2 - p1, 0.0f);
	glm::vec3 p1q1 = glm::vec3(q1 - p1, 0.0f);
	glm::vec3 p1q2 = glm::vec3(q2 - p1, 0.0f);

	float t2 = glm::dot(glm::cross(p1p2, p1q1), glm::cross(p1p2, p1q2));

	if (t1 <= 0 && t2 <= 0)
		return true;
	else
		return false;
}

bool CalcLineCrossPoint(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2, glm::vec2& crossP)
{
	if (LineRectCrossTest(p1, p2, q1, q2))
	{
		if (LineSegmentCrossTest(p1, p2, q1, q2))
		{
			// 解线性方程
			float tmpLeft, tmpRight;

			tmpLeft		= (q2.x - q1.x) * (p1.y - p2.y) - (p2.x - p1.x) * (q1.y - q2.y);
			tmpRight	= (p1.y - q1.y) * (p2.x - p1.x) * (q2.x - q1.x) + q1.x * (q2.y - q1.y) * (p2.x - p1.x) - p1.x * (p2.y - p1.y) * (q2.x - q1.x);
			crossP.x	= tmpRight / tmpLeft;

			tmpLeft		= (p1.x - p2.x) * (q2.y - q1.y) - (p2.y - p1.y) * (q1.x - q2.x);
			tmpRight	= p2.y * (p1.x - p2.x) * (q2.y - q1.y) + (q2.x - p2.x) * (q2.y - q1.y) * (p1.y - p2.y) - q2.y * (q1.x - q2.x) * (p2.y - p1.y);
			crossP.y	= tmpRight / tmpLeft;
			
			return true;
		}
	}
	return false;
}
