#pragma once
#include "VectorMath.h"
#include "Ray.h"
#include "Sampling.h"

class Camera
{
public:
	Vector3f position;
	int pixelWidth, pixelHeight;
	float filmWidth, filmHeight;
	float distPerPixel;

	float focalLength;
	float lensRadius, focusDistance;
	float startTime, shutterSpeed;

	Vector3f* film;
	int* sampleCount;

	Camera(Vector3f position, float time, float shutterSpeed, int pixelWidth, int pixelHeight, float filmWidth, float focalLength, float focusDistance, float lensRadius) 
		: startTime(time), shutterSpeed(shutterSpeed), position(position), 
		pixelWidth(pixelWidth), pixelHeight(pixelHeight), 
		filmWidth(filmWidth), focalLength(focalLength),
		focusDistance(focusDistance), lensRadius(lensRadius) {
		//focalLength = height / (2 * tan((angleOfView / 2.0) * PI / 180));
		distPerPixel = filmWidth / pixelWidth;
		filmHeight = pixelHeight * distPerPixel;


		int numberOfPixels = pixelWidth * pixelHeight;
		film = new Vector3f[numberOfPixels];
		sampleCount = new int[numberOfPixels];
		for (int i = 0; i < numberOfPixels; i++)
		{
			sampleCount[i] = 0;
		}
	}

	~Camera() {
		delete[] film;
		delete[] sampleCount;
	}

	float UniformSampleShutter(float u) const {
		return startTime + shutterSpeed * u;
	}

	Ray SampleRay(float x, float y, Sampler& sampler) const {
		float time_rand = sampler.GetRandom();
		Point2f dof_rand = sampler.GetRandom2D();
		float rayTime = UniformSampleShutter(time_rand);

		float filmX = x * distPerPixel;
		float filmY = y * distPerPixel;
		Vector3f rayDirection = Normalize(Vector3f(filmX - filmWidth / 2, filmY - filmHeight / 2, focalLength));

		Point2f pointOnLens = lensRadius * UniformSampleDisk(dof_rand);
		float focus_t = focusDistance / rayDirection.z;
		Vector3f focusPoint = position + focus_t * rayDirection;

		Vector3f rayOrigin = Vector3f(pointOnLens.u + position.x, pointOnLens.v + position.y, position.z);
		rayDirection = Normalize(focusPoint - rayOrigin);

		return Ray(rayOrigin, rayDirection, rayTime);
	}

	Ray GenerateRay(int x, int y) const {
		float filmX = x * distPerPixel;
		float filmY = y * distPerPixel;

		Vector3f rayDirection = Vector3f(filmX - filmWidth / 2, filmY - filmHeight / 2, focalLength);
		Ray ray{ position, rayDirection, startTime };
		return ray;
	}

	void ColorPixel(int x, int y, Vector3f color) {
		film[x + y * pixelWidth] = color;
	}

	void AddPixelSample(int x, int y, Vector3f color) {
		int numSamples = sampleCount[x + y * pixelWidth];
		Vector3f prevColor, newColor;
		if (numSamples > 0) {
			prevColor = film[x + y * pixelWidth];
			newColor = (prevColor * (numSamples - 1) + color) / numSamples;
		}
		else {
			newColor = color;
		}
		// 1/t * [ (t-1) * prev + new ]
		film[x + y * pixelWidth] = newColor;
		sampleCount[x + y * pixelWidth] = numSamples + 1;
	}
};
