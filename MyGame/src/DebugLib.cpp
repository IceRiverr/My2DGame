
#include <iostream>
#include "DebugLib.h"

void DebugValue(std::string msg, glm::vec2 v)
{
	std::cout << msg << "\t" << v.x << "\t\t" << v.y << std::endl;
}

void DebugValue(std::string msg, float v)
{
	std::cout << msg << "\t" << v << std::endl;
}
