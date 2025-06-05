#pragma once
#include <random>
#include "Definitions.h"
#include "VectorMath.h"

class Sampler {
	std::default_random_engine engine; 
	std::uniform_real_distribution<float> uniform; //(0.f, 1.f);
public:
	Sampler() {
		std::random_device rand_device;
		engine = std::default_random_engine(rand_device());
		uniform = std::uniform_real_distribution<float>(0.f, 1.f);
	}

	float GetRandom() {
		return uniform(engine);
	}

	Point2f GetRandom2D() {
		return Point2f(uniform(engine), uniform(engine));
	}
};

Point2f UniformSampleDisk(const Point2f& p);

Vector3f UniformSampleHemisphere(const Point2f& p);

Vector3f CosineSampleHemisphere(const Point2f& p);

Vector3f UniformSampleSphere(const Point2f& p);

Vector3f UniformSampleVectorHemisphere(const Point2f& p, const Vector3f& n);

Vector3f CosineSampleVectorHemisphere(const Point2f& p, const Vector3f& n);

float UniformSpherePDF();

float UniformHemispherePDF();

float CosineHemispherePDF(float cosTheta);