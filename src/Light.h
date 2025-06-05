#pragma once
#include "VectorMath.h"

// This class is only used for point lights
class Light
{
public:
	Vector3f position;
	Vector3f color; // power

	Light(Vector3f position, Vector3f color) : position(position), color(color) {}
};
