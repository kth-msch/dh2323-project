#pragma once
#include <vector>
#include <memory>
#include <limits>
#include "Shape.h"
#include "Triangle.h"
#include "Ray.h"
#include "VectorMath.h"
#include "Light.h"
#include "Sphere.h"
#include "AnimatedSphere.h"
#include "Definitions.h"

class Light;

class Scene
{
public:
	std::vector<std::unique_ptr<Shape>> shapes;
	std::vector<std::unique_ptr<Light>> lights;

	void AddShape(const Triangle& tri) {
		shapes.push_back(std::make_unique<Triangle>(tri));
	}

	void AddShape(const Sphere& sph) {
		shapes.push_back(std::make_unique<Sphere>(sph));
	}

	void AddShape(const LinearAnimatedSphere& sph) {
		shapes.push_back(std::make_unique<LinearAnimatedSphere>(sph));
	}

	void AddShape(const CustomAnimatedSphere& sph) {
		shapes.push_back(std::make_unique<CustomAnimatedSphere>(sph));
	}

	void AddLight(const Light& light) {
		lights.push_back(std::make_unique<Light>(light));
	}

	std::optional<ShapeIntersection> ClosestIntersection(const Ray& ray) const {
		std::optional<ShapeIntersection> closest;
		float t_min = std::numeric_limits<float>::max();
		float t;
		for (auto& shape_ptr : shapes) {
			std::optional<ShapeIntersection> isct = shape_ptr->Intersect(ray);
			if (isct) {
				t = isct->tHit;
				if (t < t_min) {
					t_min = t;
					closest = isct;
				}
			}
		}
		return closest;
	}

	 Vector3f LambertianIllumination(const ShapeIntersection& isct) const {
		 Vector3f reflected;
		 for (auto& light : lights) {
			 Vector3f partialReflected;

			 Vector3f vecToLight = light->position - isct.position;
			 Ray rayToLight{ isct.position, vecToLight, isct.time };

			 // look for shadows
			 std::optional<ShapeIntersection> light_isct = ClosestIntersection(rayToLight);
			 if (light_isct) {
				 if (light_isct->tHit < 1.f) {
					 // in shadow
					 continue;
				 }
			 }
			 Vector3f irradiance = light->color / (float)(4 * PI * vecToLight.LengthSquared());
			 float dot_product = Dot(Normalize(vecToLight), Normalize(isct.normal));
			 partialReflected = irradiance * std::max(dot_product, 0.f);

			 reflected += partialReflected;
		 }
		 Vector3f ambient = Vector3f(0.5f, 0.5f, 0.5f);
		 Vector3f intensity = Hadamard(isct.material.diffuseReflection, ambient + reflected);
		 return intensity;
	 }

	 Vector3f PhongIllumination(const ShapeIntersection& isct) const {
		 Vector3f total;
		 return total;
	 }
};
