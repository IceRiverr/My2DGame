
#pragma once
#include <glm\glm.hpp>
// BoundingBox, BBox;
struct BBox
{
	glm::vec2 pos;
	glm::vec2 size;
};

bool Collide2D(float x, float y, const BBox& bb);

bool Collide2D(const BBox& b1, const BBox& b2);

// 直线排斥实验
bool LineRectCrossTest(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2);

// 跨立判断
bool LineSegmentCrossTest(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2);

// 
bool CalcLineCrossPoint(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2, glm::vec2& crossP);