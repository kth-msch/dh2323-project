#include <cmath>
#include <algorithm>
#include "Sampling.h"
#include "VectorMath.h"

// Physically Based Rendering (3rd ed., 2018) is the primary source for the methods used here

Point2f UniformSampleDisk(const Point2f& p) {
	float r = std::sqrt(p.u);
	float theta = 2 * PI * p.v;
	return Point2f(r * std::cos(theta), r * std::sin(theta));
}

Vector3f UniformSampleHemisphere(const Point2f& p) {
	float z = p.v;
	float r = std::sqrt(std::max(0.f, 1.f - z * z));
	float phi = 2 * PI * p.u;
	return Vector3f(r * std::cos(phi), r * std::sin(phi), z);
}

Vector3f CosineSampleHemisphere(const Point2f& p) {
	// Malley's method
	Point2f d = UniformSampleDisk(p);
	float z = std::sqrt(std::max(0.f, 1.f - d.u * d.u - d.v * d.v));
	return Vector3f(d.u, d.v, z);
}

Vector3f UniformSampleSphere(const Point2f& p) {
	float z = 1 - 2 * p.u;
	float r = std::sqrt(1 - z * z);
	float phi = 2 * PI * p.v;
	return Vector3f(r * std::cos(phi), r * std::sin(phi), z);
}

Vector3f UniformSampleVectorHemisphere(const Point2f& p, const Vector3f& n) {
	Vector3f w = Normalize(n);
	Vector3f u;
	if (std::abs(w.x) > 0.1f) {
		u = Normalize(Cross(Vector3f(0, 1, 0), w));
	}
	else {
		u = Normalize(Cross(Vector3f(1, 0, 0), w));
	}
	Vector3f v = Cross(w, u);

	Vector3f r = UniformSampleHemisphere(p);
	
	Vector3f d = Normalize(u * r.x + v * r.y + w * r.z);

	return d;
}

Vector3f CosineSampleVectorHemisphere(const Point2f& p, const Vector3f& n) {
	Vector3f w = Normalize(n);
	Vector3f u;
	if (std::abs(w.x) > 0.1f) {
		u = Normalize(Cross(Vector3f(0, 1, 0), w));
	}
	else {
		u = Normalize(Cross(Vector3f(1, 0, 0), w));
	}
	Vector3f v = Cross(w, u);

	Vector3f r = CosineSampleHemisphere(p);

	Vector3f d = Normalize(u * r.x + v * r.y + w * r.z);

	return d;
}

float UniformSpherePDF() {
	return 1 / (4 * PI);
}

float UniformHemispherePDF() {
	return 1 / (2 * PI);
}

float CosineHemispherePDF(float cosTheta) {
	return cosTheta / PI;
}