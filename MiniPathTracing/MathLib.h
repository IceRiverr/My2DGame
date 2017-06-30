#pragma once
#include <vector>

#define  PI 3.1415926535898

// ÓÒÊÖ×ø±êÏµ

typedef unsigned char uchar;

class IShape;
class Entity;
struct InsterestResult;

namespace Math
{
	float Clamp(float a, float lhs, float rhs);
	float Min(float a, float b);
	float Max(float a, float b);
	int Min(int a, int b);
	int Max(int a, int b);
}

class Color
{
public:
	uchar r, g, b;

	Color();
	Color(uchar r, uchar g, uchar b);
	Color&	AddSafe(const Color& c);
	Color	operator*(float a) const;
	Color	operator+(const Color& c) const;
};

class Vec3
{
public:
	float x, y, z;

	Vec3();
	Vec3(const float x, const float y, const float z);

	Vec3	operator-() const;
	Vec3&	operator=(const Vec3& v);
	float	operator*(const Vec3& v) const;
	Vec3	operator*(const float a) const;
	Vec3	operator/(const float a) const;
	Vec3	operator+(const Vec3& v) const;
	Vec3	operator-(const Vec3& v) const;
	Vec3&	operator+=(const Vec3& v);
	Vec3&	operator-=(const Vec3& v);
	Vec3&	operator*=(const float a);
	Vec3&	operator/=(const Vec3& v);
	Vec3&	operator/=(const float a);
	
	friend Vec3 operator*(const float a, const Vec3& v);

	Vec3	Cross(const Vec3& v) const;
	float	Length() const;
	float	LengthSqr() const;
	Vec3&	Normalize();
};

struct Ray
{
	Ray(const Vec3& o, const Vec3& d);
	Vec3 origin;
	Vec3 direction;

	static Vec3 getPoint(Ray r,float t);
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
	virtual void intersect(const Ray& ray, InsterestResult& result) = 0;
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

	void intersect(const Ray& ray, InsterestResult& result);
public:
	IShape*		shape;
	TYPE		type;
	Color		color;
};

struct InsterestResult
{
	InsterestResult();

	bool			bHit;
	Entity*			entity;
	float			distance;
	Vec3			position;
	Vec3			normal;
};

class Sphere : public IShape
{
public:
	Sphere(const Vec3& o, float r);
	~Sphere() {}
	void intersect(const Ray& ray, InsterestResult& result);
	TYPE getType() { return type; }

public:
	Vec3		center;
	float		radius;
private:
	TYPE		type;
};

class Plane : public IShape
{
public:
	Plane(const Vec3& n, const Vec3& p);
	~Plane() {}
	void intersect(const Ray& ray, InsterestResult& result);
	TYPE getType() { return type; }

public:
	Vec3		normal;
	Vec3		point;

private:
	TYPE type;
};

class Camera
{
public:
	Camera(const Vec3& _eye, const Vec3& _front, const Vec3& _up, float _fov, float ratio);
	static Ray generateRay(const Camera& camera, float x, float y);

public:
	Vec3	eye;
	Vec3	front;
	Vec3	up;
	Vec3	right;
	float	fov;
	float	fovScale;
	float	aspectRatio;
};

class Scene
{
public:
	void intersect(const Ray& ray, InsterestResult& result);
	Color rayTrace(const Ray& ray, InsterestResult& hitResult);

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

