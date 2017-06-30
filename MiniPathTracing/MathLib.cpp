#define  _CRT_SECURE_NO_WARNINGS
#include "MathLib.h"
#include <math.h>
#include <stdlib.h>

Color::Color()
	: r(0), g(0), b(0)
{
	
}

Color::Color(uchar _r, uchar _g, uchar _b)
	: r(_r), g(_g), b(_b)
{

}

Color& Color::add(uchar _r, uchar _g, uchar _b)
{
	int test = this->r + _r;
	test = (test > 255) ? 255 : test;
	this->r = test;

	test = this->g + _g;
	test = (test > 255) ? 255 : test;
	this->g = test;

	test = this->b + _b;
	test = (test > 255) ? 255 : test;
	this->b = test;
	return *this;
}

Vector3::Vector3()
	: x(0.0f), y(0.0f), z(0.0f)
{

}

Vector3::Vector3(float _x, float _y, float _z)
	: x(_x), y(_y), z(_z)
{
	
}

float Vector3::length(const Vector3& v)
{
	return (float)sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float Vector3::sqrLength(const Vector3& v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Vector3::normalize(const Vector3& v)
{
	float len = Vector3::length(v);
	if (len == 0)
	{
		return v;
	}
	else
	{
		float inv = 1.0f / len;
		return Vector3(v.x * inv , v.y * inv, v.z * inv);
	}
}

Vector3 Vector3::negate(const Vector3& v)
{
	return Vector3(-v.x, -v.y, -v.z);
}

Vector3 Vector3::add(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3 Vector3::sub(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3 Vector3::mul(const Vector3& v, float m)
{
	return Vector3(v.x * m, v.y * m, v.z * m);
}

Vector3 Vector3::divide(const Vector3& v, float m)
{
	return Vector3(v.x / m, v.y / m, v.z / m);
}

float Vector3::dot(const Vector3& v1, const Vector3& v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

Vector3 Vector3::cross(const Vector3& v1, const Vector3& v2)
{
	return Vector3(v1.y * v2.z - v2.y * v1.z, 
				   v1.z * v2.x - v2.z * v1.x, 
				   v1.x * v2.y - v2.x * v1.y);
}

Ray::Ray(const Vector3& o, const Vector3& d)
	: origin(o)
	, direction(d)
{

}

Vector3 Ray::getPoint(Ray r, float t)
{
	return Vector3::add(r.origin, Vector3::mul(r.direction, t));
}

Sphere::Sphere(const Vector3& o, float r)
	: center(o), radius(r), type(IShape::SPHERE)
{
	
}

bool Sphere::intersect(const Ray& ray, InsterestResult& result)
{
	Vector3 v = Vector3::sub(ray.origin, this->center);

	float a0 = Vector3::sqrLength(v) - this->radius *  this->radius;

	float DdotV = Vector3::dot(ray.direction, v);

	if (DdotV <= 0.0f)
	{
		float discr = DdotV * DdotV - a0;
		if (discr > 0.0f)
		{
			result.distance = -DdotV - (float)sqrt(discr);
			result.position = Ray::getPoint(ray, result.distance);
			result.normal = Vector3::normalize(Vector3::sub(result.position, this->center));
			return true;
		}
	}
	return false;
}

InsterestResult::InsterestResult()
{
	entity = nullptr;
	distance = 0.0f;
	position = Vector3();
	normal = Vector3();
}

PerspectiveCamera::PerspectiveCamera(const Vector3& _eye, const Vector3& _front, const Vector3& _up, float _fov)
	: eye(_eye)
	, front(_front)
	, up(_up)
	, fov(_fov)
{
	fovScale = (float)tan(fov * 0.5f * PI / 180.0f);
	right = Vector3::cross(front, up);
}

// (0.0f, 1.0f)
Ray PerspectiveCamera::generateRay(const PerspectiveCamera& camera, float x, float y)
{
	Vector3 r = Vector3::mul(camera.right, camera.fovScale * (x - 0.5f) * 2.0f);
	Vector3 u = Vector3::mul(camera.up, camera.fovScale * (y - 0.5f) * 2.0f);
	return Ray(camera.eye, Vector3::normalize(Vector3::add(Vector3::add(camera.front, r), u)));
}

bool Scene::intersect(const Ray& ray, InsterestResult& result)
{
	InsterestResult testResult;
	float closestDist = FLT_MAX;
	bool bTestHit = false;
	bool bHit = false;
	for (int i = 0; i < (int)entities.size(); ++i)
	{
		Entity* en = entities[i];
		bTestHit = en->intersect(ray, testResult);
		if (bTestHit && testResult.distance < closestDist)
		{
			bHit = bTestHit;
			closestDist = testResult.distance;
			result.entity = testResult.entity;
			result.distance = testResult.distance;
			result.normal = testResult.normal;
			result.position = testResult.position;
		}
	}
	return bHit;
}

ppmImage::ppmImage(int _w, int _h, std::string _name)
	: w(_w), h(_h), colorBuffer(nullptr), name(_name)
{
	colorBuffer = new Color[w*h];

	for (int i = 0; i < w*h; ++i)
	{
		colorBuffer[i] = Color();
	}
}


ppmImage::~ppmImage()
{
	delete[] colorBuffer;
	colorBuffer = nullptr;
}

void ppmImage::drawPoint(int i, int j, const Color& c)
{
	colorBuffer[j * w + i] = c;
}

void ppmImage::output()
{
	std::string file = "frame//";
	file.append(name).append(".ppm");
	FILE *image = fopen(file.c_str(), "w");         // Write image to PPM file.
	fprintf(image, "P3\n%d %d\n%d\n", w, h, 255);
	for (int i = 0; i < w*h; i++)
		fprintf(image, "%d %d %d ", colorBuffer[i].r, colorBuffer[i].g, colorBuffer[i].b);
	fclose(image);
}

Plane::Plane(const Vector3& n, const Vector3& p)
	: normal(n), point(p), type(IShape::PLANE)
{

}

bool Plane::intersect(const Ray& ray, InsterestResult& result)
{
	float d = Vector3::dot(this->normal, ray.direction);
	if (d == 0.0f)
	{
		// notice
		/*result.distance = 0.0f;
		result.normal = this->normal;
		result.position = ray.origin;
		return true;*/
		// 射线和平面平行；
		return false;
	}
	else
	{
		float n = Vector3::dot(this->normal, this->point) - Vector3::dot(this->normal, ray.origin);
		float t = n / d;
		if (t <= 0.0f)
			return false;
		else
		{
			result.distance = t;
			result.normal = this->normal;
			result.position = Ray::getPoint(ray, t);
			return true;
		}
	}
}

Entity::Entity()
{

}

Entity::~Entity()
{

}

bool Entity::intersect(const Ray& ray, InsterestResult& result)
{
	if (this->shape->intersect(ray, result))
	{
		result.entity = this;
		return true;
	}
	else
	{
		return false;
	}
}
