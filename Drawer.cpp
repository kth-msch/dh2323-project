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
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
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

	// testing
	//Vector3f vec(1, 1, 1);
	//Point2f p(0.4, 1);
	//Vector3f u = UniformSampleVectorHemisphere(p, vec);
	//cout << "Vector hemisphere sample:" << u.x << "," << u.y << "," << u.z << endl;


	// load scene
	Scene scene;
	LoadScene(scene);

	// setup camera
	Vector3f cameraPosition = Vector3f(0, 0, -2);
	//float focalLength = SCREEN_HEIGHT / 2;


	float cameraStartTime = 0.f;
	float shutterSpeed = 1.f;

	//float angleOfView = 90.f; // deg
	float focusDistance = 2.f; // focus 2 units in front of camera
	//float fNumber = 32.f;
	//float lensRadius = 0.1;

	float filmWidth = 0.035;
	float focalLength = 0.020;
	float lensRadius = 0.1;

	//float fNumber = 1;
	//lensRadius = (focalLength / fNumber) / 2; // diameter = focal length / f-number

	// 	Camera(Vector3f position, float time, float shutterSpeed, int pixelWidth, int pixelHeight, float filmWidth, float focalLength, float focusDistance, float fNumber) 
	Camera camera = Camera(cameraPosition, cameraStartTime, shutterSpeed, SCREEN_HEIGHT, SCREEN_WIDTH, filmWidth, focalLength, focusDistance, lensRadius);

	cout << "Set-up complete" << endl;
	cout << "Rendering... " << endl;
	int t = SDL_GetTicks();

	// render scene
	Sampler sampler;
	int samplesPerPixel = 128;
	for (int i = 0; i < samplesPerPixel; i++)
	{
		int st = SDL_GetTicks();
		RenderSample(camera, scene, sampler);
		int sdt = SDL_GetTicks() - st;
		//Draw(camera);
		cout << i + 1 << "/" << samplesPerPixel << " samples rendered (" << sdt << " ms)" << endl;
	}


	int dt = SDL_GetTicks() - t;
	cout << "Render complete (" << dt << " ms)" << endl;
	cout << "The image will now be drawn continuously" << endl;
	cout << "Close the SDL window to save a screenshot" << endl;

	// draw image
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

	/*
	scene.AddSphere(
		Sphere(
			Vector3f(0, 0, 0),
			0.3f,
			Material(
				Vector3f(1, 0, 0),
				Vector3f(0, 0, 0),
				Vector3f(0, 0, 0)
			)
		)
	);
	*/

	// moving sphere
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

	/*
	scene.AddTriangle(
		Triangle(
			Vector3f(0, 0, -1),
			Vector3f(0, 0.5, -1),
			Vector3f(0.5, 0.5, -1),
			Vector3f(1, 1, 1),
			Vector3f(1, 1, 1)
		)
	);
	*/

	Material whiteLight(
		Vector3f(1, 1, 1),
		Vector3f(1, 1, 1) * 70.f
	);

	/*
	// emissive sphere
	scene.AddSphere(
		Sphere(
			Vector3f(0, -0.5, -0.7),
			0.3f,
			Material(
				Vector3f(1, 1, 1),
				Vector3f(1, 1, 1) * 14.f
			)
		)
	);
	*/
		
	/*
	// point light
	scene.AddLight(
		Light(
			Vector3f(0, -0.5, -0.7),
			Vector3f(1, 1, 1) * 14.f
		)
	);
	*/

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

	// moving emissive sphere
	scene.AddShape(
		CustomAnimatedSphere(
			spiral,
			0.1f,
			whiteLight
		)
	);
	
	return;
}
