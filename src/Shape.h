#pragma once
#include "VectorMath.h"
#include "Ray.h"
#include "Material.h"
#include <optional>

class ShapeIntersection
{
public:
	Vector3f position;
	Vector3f normal;
	Material material;
	float tHit;
	float time;
};

class Shape
{
public:
	Material material;

	virtual std::optional<ShapeIntersection> Intersect(const Ray& ray) = 0;
protected:
	Shape(Material material) : material(material) {}
};
