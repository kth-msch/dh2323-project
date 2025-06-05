#pragma once
#include "Sphere.h"
#include <algorithm>
#include <functional>

class LinearAnimatedSphere : public Sphere {

private:
	Vector3f start, end;
	float startTime, endTime;
public:

	LinearAnimatedSphere(Vector3f start, Vector3f end, float startTime, float endTime, float radius, Material material)
		: start(start), end(end), startTime(startTime), endTime(endTime), Sphere(start, radius, material) {}

	Vector3f GetOrigin(float time) override {
		time = std::clamp(time - startTime, 0.f, endTime - startTime) / (endTime - startTime);
		return start * (1 - time) + end * (time);
	}
};

class CustomAnimatedSphere : public Sphere {
private:
	std::function<Vector3f(float)> animation;
public:
	CustomAnimatedSphere(std::function<Vector3f(float)> animation, float radius, Material material) : animation(animation), Sphere(animation(0.f), radius, material) {}

	Vector3f GetOrigin(float time) override {
		return animation(time);
	}
};
