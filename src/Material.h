#pragma once
#include "VectorMath.h"

class Material {
public:
	Vector3f diffuseReflection;
	Vector3f specularReflection;
	Vector3f transmission;
	Vector3f emittance;

	Material() {}
	Material(Vector3f color) : diffuseReflection(color) {}
	Material(Vector3f color, Vector3f specular, Vector3f transmission) : diffuseReflection(color), specularReflection(specular), transmission(transmission) {}
	Material(Vector3f color, Vector3f emission) : diffuseReflection(color), emittance(emission) {}

	// Place to implement BRDFs!
	// Whitted 1980: should be functions that incorporate an approximation of the Fresnel reflection law 
	// (i.e., the coefficients should vary as a function of incidence angle in a manner that depends on the material's surface properties).
};
