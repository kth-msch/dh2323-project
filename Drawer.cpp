// Based on the DH2323 Lab 1 skeleton code for SDL2 interfacing
// * SDL2 (https://www.libsdl.org/)
// * C++, std::vector and glm::vec3 (https://glm.g-truc.net)

#include <iostream>
#include <memory>
#include <exception>
#include <glm/glm.hpp>
#include <vector>
#include "SDL2Auxiliary.h"

#include "Definitions.h"
#include "Scene.h"
#include "Triangle.h"
#include "Camera.h"
#include "PathTracer.h"
#include "CornellBox.h"

using namespace std;

// ---------------------------------------------------------
// GLOBAL VARIABLES
const int SCREEN_WIDTH = 250;
const int SCREEN_HEIGHT = 250;
const int NUM_SAMPLES = 16;
SDL2Aux *sdlAux;

// ---------------------------------------------------------
// FUNCTION DECLARATIONS
void Draw(Camera& camera);
void LoadScene(Scene& scene);
// ---------------------------------------------------------

// FUNCTION DEFINITIONS
int main(int argc, char* argv[])
{

	cout << "Setting up... ";
	
	// Load scene
	Scene scene;
	LoadScene(scene);

	// Setup camera
	Vector3f staticPosition = Vector3f(0, 0, -2);

	std::function<Vector3f(float)> movingPosition = [](float time) {
		float x = time / 5.f - 0.1f;
		return Vector3f(x, 0, -2);
		};

	// Temporal characteristics
	float cameraStartTime = 0.f;
	float shutterSpeed = 1.f;

	// Focus 2 units in front of camera
	float focusDistance = 2.f; 

	
	float filmWidth = 0.035;
	float focalLength = 0.020;
	float lensRadius = 0.1;

	Camera camera = Camera(movingPosition, cameraStartTime, shutterSpeed, SCREEN_HEIGHT, SCREEN_WIDTH, filmWidth, focalLength, focusDistance, lensRadius);

	cout << "Set-up complete" << endl;
	cout << "Rendering... " << endl;
	int t = SDL_GetTicks();

	// Render scene
	Sampler sampler;
	for (int i = 0; i < NUM_SAMPLES; i++)
	{
		int st = SDL_GetTicks();
		RenderSample(camera, scene, sampler);
		int sdt = SDL_GetTicks() - st;
		cout << i + 1 << "/" << NUM_SAMPLES << " samples rendered (" << sdt << " ms)" << endl;
	}

	int dt = SDL_GetTicks() - t;
	cout << "Render complete (" << dt << " ms)" << endl;
	cout << "The image will now be drawn continuously" << endl;
	cout << "Close the SDL window to save a screenshot" << endl;

	// Draw image
	sdlAux = new SDL2Aux(SCREEN_WIDTH, SCREEN_HEIGHT);
	Draw(camera);
	sdlAux->saveBMP("screenshot.bmp");

	while (!sdlAux->quitEvent()) { 
		Draw(camera);
	}
	sdlAux->saveBMP("screenshot.bmp");
	return 0;
}

void Draw(Camera& camera)
{
	sdlAux->clearPixels();

	for (int x = 0; x < camera.pixelWidth; x++)
	{
		for (int y = 0; y < camera.pixelHeight; y++)
		{
			Vector3f color = camera.film[x + y * camera.pixelWidth];
			sdlAux->putPixel(x, y, glm::vec3(color.x, color.y, color.z));
		}		
	}
	sdlAux->render();
}

void LoadScene(Scene& scene) {
	vector<Triangle> tris;
	LoadTestModel(tris); // Cornell box
	for each(Triangle tri in tris)
	{
		scene.AddShape(tri);
	}

	// Below are various shapes that are added to the Cornell box to demonstrate and test different aspects of the renderer

	/*
	* // Static sphere
	scene.AddSphere(
		Sphere(
			Vector3f(0, 0, 0),
			0.3f,
			Material(
				Vector3f(1, 0, 0)
			)
		)
	);
	*/

	/*
	// Slightly moving red sphere (right to left)
	scene.AddShape(
		LinearAnimatedSphere(
			Vector3f(0.1f, 0, 0),
			Vector3f(-0.1f, 0, 0),
			0,
			1,
			0.3f,
			Material(
				Vector3f(1, 0, 0)
			)
		)
	);
	*/

	// Large emissive sphere
	scene.AddShape(
		Sphere(
			Vector3f(0, -0.5, -0.7),
			0.3f,
			Material(
				Vector3f(1, 1, 1),
				Vector3f(1, 1, 1) * 14.f
			)
		)
	);
	
		
	/*
	// Point light
	scene.AddLight(
		Light(
			Vector3f(0, -0.5, -0.7),
			Vector3f(1, 1, 1) * 14.f
		)
	);
	*/

	/*
	// Spiralling emissive sphere

	auto spiral = [](float time) {
		float startTime = 0.f;
		float endTime = 1.f;
		time = std::clamp(time - startTime, 0.f, endTime - startTime) / (endTime - startTime);
		float angle = 2 * time * 2 * PI;
		float x = std::cos(angle);
		float z = std::sin(angle);
		float y = 2 * time - 1;

		return Vector3f(x, y, z);
	};
	Material whiteLight(
		Vector3f(1, 1, 1),
		Vector3f(1, 1, 1) * 70.f
	);
	scene.AddShape(
		CustomAnimatedSphere(
			spiral,
			0.1f,
			whiteLight
		)
	);
	*/

	/*
	// Spiralling red sphere

	auto spiral2 = [](float time) {
		float startTime = 0.f;
		float endTime = 1.f;
		time = std::clamp(time - startTime, 0.f, endTime - startTime) / (endTime - startTime);
		float angle = 2 * time * 2 * PI + PI;
		float x = std::cos(angle);
		float z = std::sin(angle);
		float y = 2 * time - 1;

		return Vector3f(x, y, z);
		};

	scene.AddShape(
		CustomAnimatedSphere(
			spiral2,
			0.1f,
			Material(
				Vector3f(1, 0, 0)
			)
		)
	);
	*/

	// Slightly moving red sphere (left to right)
	scene.AddShape(
		LinearAnimatedSphere(
			Vector3f(-0.1f, 0, 0),
			Vector3f(0.1f, 0, 0),
			0.f,
			1.f,
			0.2f,
			Material(
				Vector3f(1, 0, 0)
			)
		)
	);
	
	return;
}
