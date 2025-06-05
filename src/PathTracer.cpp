#include <iostream>
#include <optional>
#include <vector>
#include <random>
#include "Shape.h"
#include "Triangle.h"
#include "Ray.h"
#include "Camera.h"
#include "Light.h"
#include "Scene.h"
#include "PathTracer.h"

using namespace std;

const int MAX_DEPTH = 5;

Vector3f TracePath(const Scene& scene, Sampler& sampler, const Ray& ray, int depth) {
	std::optional<ShapeIntersection> isct = scene.ClosestIntersection(ray);
	if (!isct) {
		return Vector3f(0, 0, 0);
	}

	Vector3f emittedLight = isct->material.emittance;

	if (depth >= MAX_DEPTH) {
		return emittedLight;
	}

	// Adjust normal depending on which side of the shape was hit
	Vector3f corrected_normal = isct->normal;
	if (Dot(ray.direction, isct->normal) > 0) {
		corrected_normal = -corrected_normal;
	}

	// Various ways to sample directions, see: Sampling.cpp
	Vector3f s = UniformSampleSphere(sampler.GetRandom2D());
	Vector3f h = UniformSampleVectorHemisphere(sampler.GetRandom2D(), corrected_normal);
	Vector3f c = CosineSampleVectorHemisphere(sampler.GetRandom2D(), corrected_normal);

	Vector3f incomingDirection = c;

	float cos_theta = std::abs(Dot(incomingDirection, corrected_normal));
	
	//float pdf = UniformSpherePDF();
	//float pdf = UniformHemispherePDF();
	float pdf = CosineHemispherePDF(cos_theta);

	Vector3f bsdf_value;
	if (Dot(incomingDirection, corrected_normal) > 0) {
		bsdf_value = isct->material.diffuseReflection / (PI); // changed from /Pi to /2Pi // why?
	}

	Vector3f isct_point = isct->position;

	Vector3f beta = bsdf_value * std::abs(Dot(incomingDirection, corrected_normal)) / pdf;

	Ray incomingRay(isct_point, incomingDirection, isct->time);
	Vector3f incomingLight = TracePath(scene, sampler, incomingRay, depth + 1);

	Vector3f pixelColor = emittedLight + Hadamard(incomingLight, beta);
	return pixelColor;
}

Vector3f SimpleRayCast(const Scene& scene, const Ray& ray) {
	std::optional<ShapeIntersection> isct = scene.ClosestIntersection(ray);
	Vector3f pixelColor;
	if (isct) {
		pixelColor = scene.LambertianIllumination(*isct);
	}
	else {
		pixelColor = Vector3f(0, 0, 0);
	}
	return pixelColor;
}

Vector3f WhittedRayTracing(const Scene& scene, const Ray& ray, int depth) {
	if (depth >= MAX_DEPTH) {
		return Vector3f(0, 0, 0);
	}

	std::optional<ShapeIntersection> isct = scene.ClosestIntersection(ray);
	if (!isct) {
		return Vector3f(0, 0, 0);
	}

	Vector3f normal = isct->normal;
	float kn = 1.33; // index of refraction

	// Adjust normal and index of refraction (see Whitted, 1980)
	if (Dot(ray.direction, normal) > 0) {
		normal = -normal;
		kn = -kn;
	}

	Vector3f V_prim = ray.direction / std::abs(Dot(ray.direction, normal));
	

	// Reflection direction
	Vector3f R = V_prim + normal * 2;

	// Transmission direction
	float kf = 1 / std::sqrt(kn * kn * V_prim.LengthSquared() - (V_prim + normal).LengthSquared());
	Vector3f P = (normal + V_prim) * kf - normal;

	// Specular
	Ray reflected(isct->position, R, isct->time);
	Vector3f specular = Hadamard(isct->material.specularReflection, WhittedRayTracing(scene, reflected, depth + 1));

	// Transmission
	Ray transmitted(isct->position, P, isct->time);
	Vector3f transmission = Hadamard(isct->material.transmission, WhittedRayTracing(scene, transmitted, depth + 1));

	Vector3f intensity = scene.LambertianIllumination(*isct) + specular + transmission;
	return intensity;
}

void RenderSample(Camera& camera, const Scene& scene, Sampler& sampler) {
	float xf, yf;
	for (int x = 0; x < camera.pixelWidth; x++)
	{
		for (int y = 0; y < camera.pixelHeight; y++)
		{
			// Spatial sampling
			xf = x + sampler.GetRandom();
			yf = y + sampler.GetRandom();

			// Lens and time domain handled in the camera
			Ray ray = camera.SampleRay(xf, yf, sampler);

			/* Select preferred method here */
			//Vector3f pixelColor = WhittedRayTracing(scene, ray, 0);
			Vector3f pixelColor = TracePath(scene, sampler, ray, 0);

			// Accumulate sample to the camera film
			camera.AddPixelSample(x, y, pixelColor);
		}
	}

}
