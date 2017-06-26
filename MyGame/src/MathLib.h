
#pragma once
#include <glm\glm.hpp>

namespace Math
{
	// math
	float Min(float a, float b);
	float Max(float a, float b);
	
	// vector
	glm::vec2 Truncate(glm::vec2 in, float length);
	float LengthSqr(const glm::vec2 in);
	glm::vec2 Perp(glm::vec2 in);
	float UnitVec2ToRadian(glm::vec2 in);

	// bit operator
	int SetFlag(int flag, int bit);
	int ClearFlag(int flag, int bit);
	bool TestFlag(int flag, int bit);

	// random
	// -1 ~ 1
	double RandomUniform();
	// 0 ~ 1
	double RandomUnit();

}
