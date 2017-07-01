#define  _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "MathLib.h"
#include <iostream>
#include <vector>

const int w = 1024;
const int h = 1024;

int main()
{
	ppmImage colorImage = ppmImage(w, h, "color");

	Scene scene;

	Camera camera = Camera(Vec3(0.0, 0.0f, 100.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 1.0f, 0.0f), 120.0f, (float)w/(float)h);

	Entity sp1 = Entity();
	sp1.type = Entity::MODEL;
	sp1.shape = new Sphere(Vec3(-100.0f, 2.0f, -150.0f), 100.0f);
	sp1.color = Color(32, 128, 11);
	scene.entities.push_back(&sp1);

	Entity sp2 = Entity();
	sp2.type = Entity::MODEL;
	sp2.shape = new Sphere(Vec3(100.0f, -2.0f, -50.0f), 50.0f);
	sp2.color = Color(55, 11, 121);
	scene.entities.push_back(&sp2);

	Entity floor = Entity();
	floor.type = Entity::MODEL;
	floor.shape = new Plane(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, -50.0f, 0.0f));
	floor.color = Color(255, 0, 0);
	scene.entities.push_back(&floor);

	Entity wall = Entity();
	wall.type = Entity::MODEL;
	wall.shape = new Plane(Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, -400.0f));
	wall.color = Color(0, 100, 100);
	scene.entities.push_back(&wall);

	/*Entity wall2 = Entity();
	wall2.type = Entity::MODEL;
	wall2.shape = new PlaneV2(0.0f, 0.0f, 1.0f, -100.0f);
	wall2.color = Color(0, 100, 100);
	scene.entities.push_back(&wall2);*/

	Entity light1 = Entity();
	light1.type = Entity::LIGHT;
	light1.shape = new Sphere(Vec3(-40.0f, 200.0f, -50.0), 10.0f);
	light1.color = Color(0xff, 0xff, 0xff);
	scene.entities.push_back(&light1);

	Entity light2 = Entity();
	light2.type = Entity::LIGHT;
	light2.shape = new Sphere(Vec3(30.0f, 50.0f, 5.0f), 1.0f);
	light2.color = Color(0xff, 0xff, 0xff);
	scene.entities.push_back(&light2);

	float maxDepth = 200.0f;

	float dx = 1.0f / w;
	float dy = 1.0f / h;

	float sx = 0.0f;
	float sy = 0.0f;

	for (int j = 0; j < h; ++j)
	{
		sy = 1.0f - j * dy;
		for (int i = 0; i < w; ++i)
		{
			sx = i * dx;

			Ray ray = Camera::generateRay(camera, sx, sy);
			HitResult hitResult;
			Color TotalColor = scene.rayTrace(ray, hitResult);
			colorImage.drawPoint(i, j, TotalColor);
		}
	}
	
	colorImage.output();
	return 0;
}
