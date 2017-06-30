#define  _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include "MathLib.h"
#include <iostream>
#include <vector>

#define DEPTH	1
#define NORMAL	1

const int w = 512;
const int h = 512;

int main()
{
#if DEPTH
	ppmImage depthImage = ppmImage(w, h, "depth");
#endif
#if NORMAL
	ppmImage normalImage = ppmImage(w, h, "normal");
#endif
	
	ppmImage colorImage = ppmImage(w, h, "color");

	Scene scene;

	PerspectiveCamera camera = PerspectiveCamera(Vector3(0.0, 0.0f, 100.0f), Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 1.0f, 0.0f), 90.0f);

	Entity sp1 = Entity();
	sp1.type = Entity::MODEL;
	sp1.shape = new Sphere(Vector3(), 10.0f);
	sp1.color = Color(32, 128, 11);
	scene.entities.push_back(&sp1);

	Entity sp2 = Entity();
	sp2.type = Entity::MODEL;
	sp2.shape = new Sphere(Vector3(20.0f, 0.0f, -10.0f), 20.0f);
	sp2.color = Color(55, 11, 121);
	scene.entities.push_back(&sp2);

	Entity sp3 = Entity();
	sp3.type = Entity::MODEL;
	sp3.shape = new Sphere(Vector3(-20.0f, 20.0f, -20.0f), 10.0f);
	sp3.color = Color(111, 69, 221);
	scene.entities.push_back(&sp3);

	Entity floor = Entity();
	floor.type = Entity::MODEL;
	floor.shape = new Plane(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, -20.0f, 0.0f));
	floor.color = Color(0, 255, 0);
	scene.entities.push_back(&floor);

	Entity light1 = Entity();
	light1.type = Entity::LIGHT;
	light1.shape = new Sphere(Vector3(20.0f, 0.0f, 20.0), 2.0f);
	light1.color = Color(0xff, 0xff, 0xff);
	scene.entities.push_back(&light1);

	Entity light2 = Entity();
	light2.type = Entity::LIGHT;
	light2.shape = new Sphere(Vector3(0.0f, 30.0f, 10.0f), 2.0f);
	light2.color = Color(0xff, 0xff, 0xff);
	scene.entities.push_back(&light2);

	float maxDepth = 200.0f;

	float dx = 1.0f / h;
	float dy = 1.0f / w;

	float sx = 0.0f;
	float sy = 0.0f;

	for (int j = 0; j < h; ++j)
	{
		sy = 1.0f - j * dy;
		for (int i = 0; i < w; ++i)
		{
			sx = i * dx;

			Ray ray = PerspectiveCamera::generateRay(camera, sx, sy);
			InsterestResult result;
			bool bHit = scene.intersect(ray, result);
			if (bHit)
			{
#if DEPTH
				uchar depth = (uchar)(255.0f - result.distance / maxDepth * 255.0f);
				depthImage.drawPoint(i, j, Color(depth, depth, depth));
#endif

#if NORMAL
				Vector3 normal = result.normal;// 世界空间的法向量
				normal = Vector3(normal.x * 0.5f + 0.5f, normal.y * 0.5f + 0.5f, normal.z * 0.5f + 0.5f);
				Color c((uchar)(normal.x * 255.0f), (uchar)(normal.y * 255.0f), (uchar)(normal.z * 255.0f));
				normalImage.drawPoint(i, j, c);
#endif
				Entity* ent = result.entity;
				if (ent->type == Entity::LIGHT)
				{
					Color entityColor = ent->color;
					colorImage.drawPoint(i, j, entityColor);
				}
				else
				{
					Color entityColor;
					for (int en = 0; en < (int)scene.entities.size(); ++en)
					{
						Entity* test = scene.entities[en];
						if (test != ent && test->type == Entity::LIGHT)
						{
							Entity* light = test;
							Sphere* lightShpae = static_cast<Sphere*>(light->shape);
							Vector3 toLight = Vector3::sub(lightShpae->center, result.position);
							toLight = Vector3::normalize(toLight);
							float LdotN = Vector3::dot(result.normal, toLight);
							if (LdotN > 0.0f)
							{
								entityColor.add((uchar)(LdotN * ent->color.r), (uchar)(LdotN  * ent->color.g), (uchar)(LdotN * ent->color.b));
							}
						}
					}
					colorImage.drawPoint(i, j, entityColor);
				}
			}
		}
	}

#if DEPTH
	depthImage.output();
#endif
#if NORMAL
	normalImage.output();
#endif
	colorImage.output();

	return 0;
}