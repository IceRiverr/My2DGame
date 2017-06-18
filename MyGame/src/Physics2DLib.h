
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

struct AABB
{
	glm::vec2 LT;	// LeftTop
	glm::vec2 RB;	// RightBottom
};

bool Collide2D(float x, float y, const AABB& aabb);

bool Collide2D(const AABB& c1, const AABB& c2);


