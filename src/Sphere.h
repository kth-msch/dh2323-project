#pragma once
#include <cmath>
#include <limits>
#include "Shape.h"
#include "VectorMath.h"

class Sphere : public Shape {
private:
	Vector3f origin;
	float radius;
public:
	Sphere(Vector3f origin, float radius, Material material) : origin(origin), radius(radius), Shape(material) {}

	virtual Vector3f GetOrigin(float time) {
		return origin;
	}

	// Ray-Sphere intersection
	std::optional<ShapeIntersection> Intersect(const Ray& ray) override {
		Vector3f ray_unit_direction = Normalize(ray.direction);
		Vector3f ray_origin = ray.origin;
		float ray_time = ray.timeOfOrigin;
		Vector3f sphere_origin = GetOrigin(ray_time);

		float b = Dot(ray_unit_direction, ray_origin - sphere_origin);

		float term = b * b - ((ray_origin - sphere_origin).LengthSquared() - radius * radius);

		if (term >= 0) {
			// Up to 2 possible intersection points (into and out of sphere)
			float t1 = -b + std::sqrt(term);
			bool t1_viable = t1 > 100 * std::numeric_limits<float>::epsilon();
			float t2 = -b - std::sqrt(term);
			bool t2_viable = t2 > 100 * std::numeric_limits<float>::epsilon();
			if (t1_viable || t2_viable) {
				float t;
				if (t1_viable && t2_viable) {
					// Pick the closest intersection
					t = std::min(t1, t2);
				}
				else if (t1_viable) {
					t = t1;
				}
				else {
					t = t2;
				}
				float t_corrected = t / ray.direction.Length();
				Vector3f position = ray(t_corrected);
				Vector3f normal = Normalize(position - sphere_origin);

				return ShapeIntersection{
					position,
					normal,
					material,
					t_corrected,
					ray.timeOfOrigin
				};
			}
		}
		return {};
	}
};