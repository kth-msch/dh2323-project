#pragma once
#include "Shape.h"
#include <optional>
#include <limits>

class Triangle :
    public Shape
{
public:
	Vector3f v0, v1, v2;
    Vector3f normal;

    Triangle(Vector3f v0, Vector3f v1, Vector3f v2, Material material) : v0(v0), v1(v1), v2(v2), Shape(material) {
        ComputeNormal();
    }

    void ComputeNormal() {
        Vector3f e1 = v1 - v0;
        Vector3f e2 = v2 - v0;
        normal = Normalize(Cross(e2, e1));
    }

	// Ray-Triangle intersection
    std::optional<ShapeIntersection> Intersect(const Ray& ray) override {
		Vector3f e1 = v1 - v0;
		Vector3f e2 = v2 - v0;
		Vector3f b = ray.origin - v0;
		Matrix3f A(-(ray.direction), e1, e2);

		float A_det = A.Determinant();
		float temp_A_det = 0;
		Matrix3f temp_A = A;
		float t, u, v;
		Vector3f isct_location;
		ShapeIntersection isct;
		if (A_det != 0) {
			temp_A = Matrix3f(b, e1, e2);
			temp_A_det = temp_A.Determinant();
			t = temp_A_det / A_det;
			if (t > 100 * std::numeric_limits<float>::epsilon() ) { // better way?
				// calculate u
				temp_A = Matrix3f(-(ray.direction), b, e2);
				temp_A_det = temp_A.Determinant();
				u = temp_A_det / A_det;
				if (u >= 0) {
					// v
					temp_A = Matrix3f(-(ray.direction), e1, b);
					temp_A_det = temp_A.Determinant();
					v = temp_A_det / A_det;
					if (v >= 0 && u + v <= 1) {

						return ShapeIntersection{
							ray(t),
							normal,
							material,
							t,
							ray.timeOfOrigin
						};
					}
				}
			}
		}
		return {};
    }
};

