#ifndef TEST_MODEL_CORNEL_BOX_H
#define TEST_MODEL_CORNEL_BOX_H

// Defines a simple test model: The Cornel Box

#include <vector>
#include "VectorMath.h"

// Used to describe a triangular surface:
#include "Triangle.h"

// Loads the Cornell Box. It is scaled to fill the volume:
// -1 <= x <= +1
// -1 <= y <= +1
// -1 <= z <= +1
void LoadTestModel(std::vector<Triangle>& triangles)
{
	// Defines colors:
	Vector3f color_red(0.75f, 0.15f, 0.15f);
	Vector3f color_yellow(0.75f, 0.75f, 0.15f);
	Vector3f color_green(0.15f, 0.75f, 0.15f);
	Vector3f color_cyan(0.15f, 0.75f, 0.75f);
	Vector3f color_blue(0.15f, 0.15f, 0.75f);
	Vector3f color_purple(0.75f, 0.15f, 0.75f);
	Vector3f color_white(0.75f, 0.75f, 0.75f);

	// Materials
	Material mirror(Vector3f(0, 0, 0), Vector3f(1, 1, 1), Vector3f(0, 0, 0));
	Material transmit(Vector3f(0, 0, 0), Vector3f(0, 0, 0), Vector3f(1, 1, 1));

	Material red(color_red);
	//Material red = transmit; // prism box
	Material yellow(color_yellow);
	//Material yellow = mirror; // mirror wall
	Material green(color_green);
	Material cyan(color_cyan);
	Material blue(color_blue);
	Material purple(color_purple);
	Material white(color_white);

	triangles.clear();
	triangles.reserve(5 * 2 * 3);

	// ---------------------------------------------------------------------------
	// Room

	float L = 555;			// Length of Cornell Box side.

	Vector3f A(L, 0, 0);
	Vector3f B(0, 0, 0);
	Vector3f C(L, 0, L);
	Vector3f D(0, 0, L);

	Vector3f E(L, L, 0);
	Vector3f F(0, L, 0);
	Vector3f G(L, L, L);
	Vector3f H(0, L, L);

	// Floor:
	triangles.push_back(Triangle(C, B, A, green));
	triangles.push_back(Triangle(C, D, B, green));

	// Left wall
	triangles.push_back(Triangle(A, E, C, purple));
	triangles.push_back(Triangle(C, E, G, purple));

	// Right wall
	triangles.push_back(Triangle(F, B, D, yellow));
	triangles.push_back(Triangle(H, F, D, yellow));

	// Ceiling
	triangles.push_back(Triangle(E, F, G, cyan));
	triangles.push_back(Triangle(F, H, G, cyan));

	// Back wall
	triangles.push_back(Triangle(G, D, C, white));
	triangles.push_back(Triangle(G, H, D, white));

	// ---------------------------------------------------------------------------
	// Short block

	A = Vector3f(290, 0, 114);
	B = Vector3f(130, 0, 65);
	C = Vector3f(240, 0, 272);
	D = Vector3f(82, 0, 225);

	E = Vector3f(290, 165, 114);
	F = Vector3f(130, 165, 65);
	G = Vector3f(240, 165, 272);
	H = Vector3f(82, 165, 225);

	// Front
	triangles.push_back(Triangle(E, B, A, red));
	triangles.push_back(Triangle(E, F, B, red));

	// Front
	triangles.push_back(Triangle(F, D, B, red));
	triangles.push_back(Triangle(F, H, D, red));

	// BACK
	triangles.push_back(Triangle(H, C, D, red));
	triangles.push_back(Triangle(H, G, C, red));

	// LEFT
	triangles.push_back(Triangle(G, E, C, red));
	triangles.push_back(Triangle(E, A, C, red));

	// TOP
	triangles.push_back(Triangle(G, F, E, red));
	triangles.push_back(Triangle(G, H, F, red));

	// ---------------------------------------------------------------------------
	// Tall block

	A = Vector3f(423, 0, 247);
	B = Vector3f(265, 0, 296);
	C = Vector3f(472, 0, 406);
	D = Vector3f(314, 0, 456);

	E = Vector3f(423, 330, 247);
	F = Vector3f(265, 330, 296);
	G = Vector3f(472, 330, 406);
	H = Vector3f(314, 330, 456);

	// Front
	triangles.push_back(Triangle(E, B, A, blue));
	triangles.push_back(Triangle(E, F, B, blue));

	// Front
	triangles.push_back(Triangle(F, D, B, blue));
	triangles.push_back(Triangle(F, H, D, blue));

	// BACK
	triangles.push_back(Triangle(H, C, D, blue));
	triangles.push_back(Triangle(H, G, C, blue));

	// LEFT
	triangles.push_back(Triangle(G, E, C, blue));
	triangles.push_back(Triangle(E, A, C, blue));

	// TOP
	triangles.push_back(Triangle(G, F, E, blue));
	triangles.push_back(Triangle(G, H, F, blue));


	// ----------------------------------------------
	// Scale to the volume [-1,1]^3

	for (size_t i = 0; i < triangles.size(); ++i)
	{
		triangles[i].v0 *= 2 / L;
		triangles[i].v1 *= 2 / L;
		triangles[i].v2 *= 2 / L;

		triangles[i].v0 -= Vector3f(1, 1, 1);
		triangles[i].v1 -= Vector3f(1, 1, 1);
		triangles[i].v2 -= Vector3f(1, 1, 1);

		triangles[i].v0.x *= -1;
		triangles[i].v1.x *= -1;
		triangles[i].v2.x *= -1;

		triangles[i].v0.y *= -1;
		triangles[i].v1.y *= -1;
		triangles[i].v2.y *= -1;

		triangles[i].ComputeNormal();
	}
}

#endif