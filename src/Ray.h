#pragma once
#include "VectorMath.h"

class Ray
{
public:
	Vector3f origin;
	Vector3f direction;
	float timeOfOrigin = 0;
	//float speed = 299792458.f; // future work :)

	Ray(Vector3f origin, Vector3f direction, float time = 0.f) : origin(origin), direction(direction), timeOfOrigin(time) {}

	Vector3f operator()(float t) const {
		return origin + direction * t;
	}

};
