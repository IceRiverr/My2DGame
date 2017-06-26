
#include "MathLib.h"
#include "TypeDefine.h"
#include <random>

namespace Math
{
	float Min(float a, float b)
	{
		return a < b ? a : b;
	}

	float Max(float a, float b)
	{
		return a > b ? a : b;
	}

	glm::vec2 Truncate(glm::vec2 in, float length)
	{
		float inLen = glm::length(in);
		if (inLen <= length)
			return in;
		else
		{
			return in / inLen * length;
		}
	}

	float LengthSqr(const glm::vec2 in)
	{
		return in.x * in.x + in.y * in.y;
	}

	glm::vec2 Perp(glm::vec2 in)
	{
		glm::vec3 sideDir = glm::cross(glm::vec3(in, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec2 perp = glm::vec2(sideDir.x, sideDir.y);
		perp = glm::normalize(perp);
		return perp;
	}

	float UnitVec2ToRadian(glm::vec2 in)
	{
		float radian = 0.0f;
		if (in.y >= 0.0f)
		{
			radian = std::acos(in.x);
		}
		else
		{
			radian = (float)PI_2 - std::acos(in.x);
		}
		return radian;
	}

	int SetFlag(int flag, int bit)
	{
		return (flag |= bit);
	}

	int ClearFlag(int flag, int bit)
	{
		return (flag ^= bit);
	}

	bool TestFlag(int flag, int bit)
	{
		return ((flag & bit) == bit);
	}

	double RandomUniform()
	{
		static std::default_random_engine engine;
		static std::uniform_real_distribution<double> random(-1.0, 1.0);
		return random(engine);
	}

	double RandomUnit()
	{
		return RandomUniform() * 0.5 + 0.5;
	}

}
