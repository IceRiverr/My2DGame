#pragma once
#include <vector>

#define  PI 3.1415926535898

// ÓÒÊÖ×ø±êÏµ

typedef unsigned char uchar;

class IShape;
class Entity;
struct InsterestResult;

struct Color
{
	Color();
	Color(uchar _r, uchar _g, uchar _b);

	Color& add(uchar _r, uchar _g, uchar _b);

	uchar r, g, b;
};

struct Vector3
{
	float x, y, z;

	Vector3();
	Vector3(float _x, float _y, float _z);

	static float length(const Vector3& v);

	static float sqrLength(const Vector3& v);

	static Vector3 normalize(const Vector3& v);

	static Vector3 negate(const Vector3& v);

	static Vector3 add(const Vector3& v1, const Vector3& v2);

	static Vector3 sub(const Vector3& v1, const Vector3& v2);

	static Vector3 mul(const Vector3& v, float m);
	
	static Vector3 divide(const Vector3& v, float m);

	static float dot(const Vector3& v1, const Vector3& v2);

	static Vector3 cross(const Vector3& v1, const Vector3& v2);

};

struct Ray
{
	Ray(const Vector3& o, const Vector3& d);
	Vector3 origin;
	Vector3 direction;

	static Vector3 getPoint(Ray r,float t);
};

class IShape
{
public:
	enum TYPE
	{
		SPHERE,
		PLANE,
	};

	virtual ~IShape() {};
	virtual bool intersect(const Ray& ray, InsterestResult& result) = 0;
	virtual TYPE getType() = 0;
};

class Entity
{
public:
	enum TYPE
	{
		MODEL,
		LIGHT,
	};

	Entity();
	~Entity();

	bool intersect(const Ray& ray, InsterestResult& result);
public:
	IShape*		shape;
	TYPE		type;
	Color		color;
};

struct InsterestResult
{
	InsterestResult();

	Entity*			entity;
	float			distance;
	Vector3			position;
	Vector3			normal;
};

class Sphere : public IShape
{
public:
	Sphere(const Vector3& o, float r);
	~Sphere() {}
	bool intersect(const Ray& ray, InsterestResult& result);
	TYPE getType() { return type; }

public:
	Vector3		center;
	float		radius;
private:
	TYPE		type;
};

class Plane : public IShape
{
public:
	Plane(const Vector3& n, const Vector3& p);
	~Plane() {}
	bool intersect(const Ray& ray, InsterestResult& result);
	TYPE getType() { return type; }

public:
	Vector3		normal;
	Vector3		point;

private:
	TYPE type;
};

struct PerspectiveCamera
{
	Vector3 eye;
	Vector3 front;
	Vector3 up;
	Vector3 right;
	float	fov;
	float	fovScale;

	PerspectiveCamera(const Vector3& _eye, const Vector3& _front, const Vector3& _up, float _fov);

	static Ray generateRay(const PerspectiveCamera& camera, float x, float y);
};

class Scene
{
public:
	bool intersect(const Ray& ray, InsterestResult& result);

	std::vector<Entity*> entities;
};

class ppmImage
{
public:
	ppmImage(int _w, int _h, std::string _name);
	~ppmImage();

	void drawPoint(int i, int j, const Color& c);
	void output();
private:
	std::string name;
	int			w;
	int			h;
	Color*		colorBuffer;
};

