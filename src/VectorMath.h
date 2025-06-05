#pragma once
#include <cmath>
#include <iostream>

class Vector3f
{
public:
	Vector3f() : x(0), y(0), z(0) {}
	Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}
	float operator[](int i) const {
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	float&operator[](int i) {
		if (i == 0) return x;
		if (i == 1) return y;
		return z;
	}
	Vector3f operator+(const Vector3f& other) const {
		return Vector3f{ x + other.x, y + other.y, z + other.z };
	}
	
	Vector3f operator-(const Vector3f& other) const {
		return Vector3f{ x - other.x, y - other.y, z - other.z };
	}
	Vector3f operator-() const {
		return Vector3f{ -x, -y, -z };
	}
	
	Vector3f& operator+=(const Vector3f& other) {
		(*this) = (*this) + other;
		return *this;
	}
	Vector3f& operator-=(const Vector3f& other) {
		(*this) = (*this) - other;
		return *this;
	}

	Vector3f operator*(const float& scalar) const {
		return Vector3f{ x * scalar, y * scalar, z * scalar };
	}
	Vector3f operator/(const float& scalar) const {
		return Vector3f{ x / scalar, y / scalar, z / scalar };
	}

	Vector3f operator*=(const float& scalar) {
		(*this) = (*this) * scalar;
		return *this;
	}
	Vector3f operator/=(const float& scalar) {
		(*this) = (*this) / scalar;
		return *this;
	}

	float LengthSquared() const { return x * x + y * y + z * z; }

	float Length() const { return std::sqrt(LengthSquared()); }


	float x, y, z;
};

Vector3f Abs(const Vector3f a);

Vector3f Normalize(Vector3f v);

float Dot(Vector3f v, Vector3f w);

//float AngleBetween(Vector3f v1, Vector3f v2);

float AbsDot(Vector3f v1, Vector3f v2);

Vector3f Cross(Vector3f v, Vector3f w);

float Distance(Vector3f v1, Vector3f v2);

float DistanceSquared(Vector3f v1, Vector3f v2);

Vector3f Hadamard(Vector3f v1, Vector3f v2);

Vector3f operator*(const float& scalar, const Vector3f& v);

std::ostream& operator<<(std::ostream& os, const Vector3f& v);

class Matrix3f {
public:
	Vector3f u, v, w;

	Matrix3f() {
		u = Vector3f(1, 0, 0);
		v = Vector3f(0, 1, 0);
		w = Vector3f(0, 0, 1);
	}
	Matrix3f(Vector3f u, Vector3f v, Vector3f w) : u(u), v(v), w(w) {}

	Vector3f operator[](int i) const {
		if (i == 0) return u;
		if (i == 1) return v;
		return w;
	}

	Vector3f& operator[](int i) {
		if (i == 0) return u;
		if (i == 1) return v;
		return w;
	}

	float Determinant() const {
		float a = u[0];
		float b = v[0];
		float c = w[0];
		float d = u[1];
		float e = v[1];
		float f = w[1];
		float g = u[2];
		float h = v[2];
		float i = w[2];
		float A = a * e * i;
		float B = b * f * g;
		float C = c * d * h;
		float D = c * e * g;
		float E = b * d * i;
		float F = a * f * h;
		float determinant = A + B + C - D - E - F;
		return determinant;
	}
};

// 2D points

class Point2f {
public:
	Point2f() : u(0), v(0) {}
	Point2f(float u, float v) : u(u), v(v) {}
	float operator[](int i) const {
		if (i == 0) return u;
		return v;
	}
	float& operator[](int i) {
		if (i == 0) return u;
		return v;
	}
	Point2f operator+(const Point2f& other) const {
		return Point2f{ u + other.u, v + other.v };
	}
	Point2f operator-(const Point2f& other) const {
		return Point2f{ u - other.u, v - other.v };
	}
	Point2f operator*(const float& scalar) const {
		return Point2f{ u * scalar, v * scalar };
	}
	float u, v;
};

Point2f operator*(const float& scalar, const Point2f& p);