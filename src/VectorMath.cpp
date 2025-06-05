#include "VectorMath.h"

Vector3f Abs(const Vector3f a) {
	return Vector3f{ std::abs(a.x), std::abs(a.y), std::abs(a.z) };
}

Vector3f Normalize(Vector3f v) {
	return v / v.Length();
}

float Dot(Vector3f v, Vector3f w) {
	return v.x * w.x + v.y * w.y + v.z * w.z;
}

float AngleBetween(Vector3f v1, Vector3f v2) {
	return std::acos(Dot(v1, v2)); // technically inaccurate
}

float AbsDot(Vector3f v1, Vector3f v2) {
	return std::abs(Dot(v1, v2));
}

Vector3f Cross(Vector3f v, Vector3f w) {
	return Vector3f{
		v.y * w.z - v.z * w.y,
		v.z * w.x - v.x * w.z,
		v.x * w.y - v.y * w.x
	};
}

float Distance(Vector3f v1, Vector3f v2) {
	return (v1 - v2).Length();
}

float DistanceSquared(Vector3f v1, Vector3f v2) {
	return (v1 - v2).LengthSquared();
}

Vector3f Hadamard(Vector3f v1, Vector3f v2) {
	return Vector3f{
		v1.x * v2.x,
		v1.y * v2.y,
		v1.z * v2.z
	};
}

Vector3f operator*(const float& scalar, const Vector3f& v)
{
	return Vector3f{ v.x * scalar, v.y * scalar, v.z * scalar };
}

std::ostream& operator<<(std::ostream& os, const Vector3f& v) {
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}


Point2f operator*(const float& scalar, const Point2f& p)
{
	return Point2f{ p.u * scalar, p.v * scalar };
}
