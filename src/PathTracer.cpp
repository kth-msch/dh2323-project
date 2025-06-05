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

	//Vector3f outgoingDirection = -ray.direction;
	Vector3f corrected_normal = isct->normal;
	if (Dot(ray.direction, isct->normal) > 0) {
		corrected_normal = -corrected_normal;
	}

	Vector3f s = UniformSampleSphere(sampler.GetRandom2D());
	//cout << "Sphere sample:" << s.x << "," << s.y << "," << s.z << endl;
	Vector3f h = UniformSampleVectorHemisphere(sampler.GetRandom2D(), corrected_normal);

	/*
	if (isct->material.diffuseReflection.x == 0.75f && isct->material.diffuseReflection.y == 0.75f && isct->material.diffuseReflection.z == 0.15f) {
		cout << "Yellow debug: " << endl;
		cout << "Corrected normal: " << corrected_normal << endl;
		cout << "h: " << h << endl;
	}
	*/

	//cout << "Hemisphere sample:" << h.x << "," << h.y << "," << h.z << endl;
	Vector3f c = CosineSampleVectorHemisphere(sampler.GetRandom2D(), corrected_normal);

	Vector3f incomingDirection = h;

	float cos_theta = std::abs(Dot(incomingDirection, corrected_normal));
	//float pdf = CosineHemispherePDF(cos_theta);

	float pdf = UniformHemispherePDF();
	//float pdf = UniformSpherePDF();



	Vector3f bsdf_value;
	if (Dot(incomingDirection, corrected_normal) > 0) {
		bsdf_value = isct->material.diffuseReflection / (PI); // changed from /Pi to /2Pi // why?
	}
	
	//Vector3f 

	Vector3f isct_point = isct->position;
	//Vector3f bounceDirection = ray.direction - isct->normal * (2 * Dot(ray.direction, isct->normal)); // perfectly specular
	// for diffuse, this should be cosine-weighted hemisphere sample


	
	//float probability = 1 / (2 * PI); // hemisphere


	//Vector3f fcos = bsdf_value * cos_theta;
	Vector3f beta = bsdf_value * std::abs(Dot(incomingDirection, corrected_normal)) / pdf;
	//Vector3f BRDF = shapeColor / PI;

	Ray incomingRay(isct_point, incomingDirection, isct->time);
	Vector3f incomingLight = TracePath(scene, sampler, incomingRay, depth + 1);

	Vector3f pixelColor = emittedLight + Hadamard(incomingLight, beta);

	/*
	if (isct->material.diffuseReflection.x == 0.75f && isct->material.diffuseReflection.y == 0.75f && isct->material.diffuseReflection.z == 0.15f) {
		if (pixelColor.x != 0 && pixelColor.y != 0 && pixelColor.z != 0) {

		
		cout << "Yellow debug with non-zero pixel color" << endl;
		cout << "Emitted light: " << emittedLight << endl;
		cout << "Incoming light vector: " << incomingLight << endl;
		cout << "bsdf value: " << bsdf_value << endl;

		cout << "Incoming direction: " << incomingDirection << endl;
		cout << "Corrected normal: " << corrected_normal << endl;
		cout << "Dot product: " << Dot(incomingDirection, corrected_normal) << endl;
		cout << "beta: " << beta << endl;
		cout << "pdf: " << pdf << endl;
		cout << "Pixel value: " << pixelColor << endl;
		}
	}
	*/

	return pixelColor;
	

	//pixelColor = Hadamard(isct->color, scene.Radiance(*isct));
	
	
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

	if (Dot(ray.direction, normal) > 0) {
		normal = -normal;
		kn = -kn;
	}

	Vector3f V_prim = ray.direction / std::abs(Dot(ray.direction, normal));
	

	// reflection direction
	Vector3f R = V_prim + normal * 2; //ray.direction - isct->normal * (2 * Dot(ray.direction, isct->normal));

	// transmission direction
	float kf = 1 / std::sqrt(kn * kn * V_prim.LengthSquared() - (V_prim + normal).LengthSquared());
	// Whitted 1980
	// Since these equations assume that V- N is less than zero, 
	// the intersection processor must adjust the sign of N so that it points to the side of the surface from which the intersecting ray is incident. 
	// It must likewise adjust the index of refraction to account for the sign change.
	Vector3f P = (normal + V_prim) * kf - normal;

	// specular
	Ray reflected(isct->position, R, isct->time);
	Vector3f specular = Hadamard(isct->material.specularReflection, WhittedRayTracing(scene, reflected, depth + 1));

	// transmission
	Ray transmitted(isct->position, P, isct->time);
	Vector3f transmission = Hadamard(isct->material.transmission, WhittedRayTracing(scene, transmitted, depth + 1));

	Vector3f intensity = scene.LambertianIllumination(*isct) + specular + transmission;

	return intensity;
}

void RenderSample(Camera& camera, const Scene& scene, Sampler& sampler) {
	//float seed = 10110;
	//std::random_device rand_device; // used to seed the random engine only
	//std::default_random_engine eng(rand_device());
	//std::uniform_real_distribution<float> uniform(0.f, 1.f);
	
	//float u;
	//Vector3f random;
	float xf, yf;


	for (int x = 0; x < camera.pixelWidth; x++)
	{
		for (int y = 0; y < camera.pixelHeight; y++)
		{
			//Ray ray = camera.GenerateRay(x, y);
			//Vector3f pixelColor = WhittedRayTracing(scene, ray, 0);
			//camera.ColorPixel(x, y, pixelColor);

			//u = uniform(engine);

			// spatial supersampling
			xf = x + sampler.GetRandom();
			yf = y + sampler.GetRandom();

			// time and lens domains
			//random.x = uniform(eng);
			//random.y = uniform(eng);
			//random.z = uniform(eng);
			Ray ray = camera.SampleRay(xf, yf, sampler);

			//Vector3f pixelColor = WhittedRayTracing(scene, ray, 0);
			Vector3f pixelColor = TracePath(scene, sampler, ray, 0);

			camera.AddPixelSample(x, y, pixelColor);

			//Vector3f pixelColor = TracePath(scene, ray, 0);
			//cout << pixelColor.x << "," << pixelColor.y << "," << pixelColor.z << endl;
			//Vector3f pixelColor = SimpleRayCast(scene, ray);

			// accumulation: 1/t * ( (t-1) * prev + f(x)/p(x) )
			// 1/t * [ (t-1) * prev + new ]
			// maybe:
			// camera.AddPixelSample(x, y, color)
			// track # of samples for each pixel in Camera
		}
	}

}
