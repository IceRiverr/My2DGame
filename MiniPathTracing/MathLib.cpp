#define  _CRT_SECURE_NO_WARNINGS
#include "MathLib.h"
#include <math.h>
#include <stdlib.h>

Color::Color()
	: r(0), g(0), b(0)
{
}

Color::Color(uchar r, uchar g, uchar b)
{
	this->r = r;
	this->g = g;
	this->b = b;
}

Color& Color::AddSafe(const Color& c)
{
	int R = (int)r + (int)c.r;
	int G = (int)g + (int)c.g;
	int B = (int)b + (int)c.b;

	R = Math::Min(R, 255);
	G = Math::Min(G, 255);
	B = Math::Min(B, 255);

	r = (uchar)R;
	g = (uchar)G;
	b = (uchar)B;
	return *this;
}

Color Color::operator*(float a) const
{
	int R = (int)(r * a);
	int G = (int)(g * a);
	int B = (int)(b * a);

	Color newColor;
	newColor.r = (uchar)R;
	newColor.g = (uchar)G;
	newColor.b = (uchar)B;
	return newColor;
}

Color Color::operator+(const Color& c) const
{
	Color d = *this;
	d.AddSafe(c);
	return d;
}

Vec3::Vec3()
	: x(0.0f), y(0.0f), z(0.0f)
{
}

Vec3::Vec3(const float x, const float y, const float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vec3 Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}

Vec3 Vec3::operator-(const Vec3& v) const
{
	return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3& Vec3::operator-=(const Vec3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vec3& Vec3::operator*=(const float a)
{
	x *= a;
	y *= a;
	z *= a;
	return *this;
}

Vec3& Vec3::operator+=(const Vec3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vec3& Vec3::operator=(const Vec3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

float Vec3::operator*(const Vec3& v) const
{
	return (x * v.x + y * v.y + z * v.z);
}

Vec3 Vec3::operator*(const float a) const
{
	return Vec3(x * a, y * a, z *a);
}

Vec3 Vec3::operator/(const float a) const
{
	float inva = 1.0f / a;
	return Vec3(x * inva, y * inva, z * inva);
}

Vec3 Vec3::operator+(const Vec3& v) const
{
	return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3& Vec3::operator/=(const Vec3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

Vec3& Vec3::operator/=(const float a)
{
	float inva = 1.0f / a;
	x *= inva;
	y *= inva;
	z *= inva;
	return *this;
}

Vec3 operator*(const float a, const Vec3& v)
{
	return Vec3(a * v.x, a * v.y, a * v.z);
}

Vec3 refract(const Vec3& normal, const Vec3& incident, float n1, float n2)
{
	float n = n1 / n2;
	float cosI = normal * incident;
	float sinI2 = n * n * cosI * cosI;
	if (sinI2 > 1.0f)
		return Vec3(0.0f, 0.0f, 0.0f);
	else
	{
		return incident * n - (n * cosI + (float)sqrt(1.0f - sinI2)) * normal;
	}
}

Vec3 Vec3::Cross(const Vec3& v) const
{
	return Vec3(y * v.z - v.y * z, z * v.x - v.z * x, x * v.y - v.x * y);
}

float Vec3::Length() const
{
	return (float)sqrt(x * x + y * y + z * z);
}

float Vec3::LengthSqr() const
{
	return (x * x + y * y + z * z);
}

Vec3& Vec3::Normalize()
{
	float invLength = 1.0f / (float)sqrt(x * x + y * y + z * z);
	x *= invLength;
	y *= invLength;
	z *= invLength;
	return *this;
}

Ray::Ray(const Vec3& o, const Vec3& d)
	: origin(o)
	, direction(d)
{
}

Vec3 Ray::getPoint(Ray r, float t)
{
	return r.origin + (r.direction * t);
}

Sphere::Sphere(const Vec3& o, float r)
	: center(o), radius(r), type(IShape::SPHERE)
{
}

void Sphere::intersect(const Ray& ray, HitResult& result)
{
	Vec3 v = ray.origin - this->center;

	float a0 = v.LengthSqr() - this->radius *  this->radius;

	float DdotV = ray.direction * v;

	if (DdotV <= 0.0f)
	{
		float discr = DdotV * DdotV - a0;
		if (discr > 0.0f)
		{
			float dist = -DdotV - (float)sqrt(discr);
			if (dist > 0.0f && dist < TRACE_DIST)
			{
				result.bHit = true;
				result.distance = dist;
				result.position = Ray::getPoint(ray, result.distance);
				result.normal = (result.position - this->center).Normalize();
			}
			else
			{
				dist = -DdotV + (float)sqrt(discr);
				result.bHit = true;
				result.distance = dist;
				result.position = Ray::getPoint(ray, result.distance);
				result.normal = (result.position - this->center).Normalize();
			}
		}
	}
}

HitResult::HitResult()
{
	bHit = false;
	entity = nullptr;
	distance = 0.0f;
	position = Vec3();
	normal = Vec3();
}

Camera::Camera(const Vec3& _eye, const Vec3& _front, const Vec3& _up, float _fov, float ratio)
	: eye(_eye)
	, front(_front)
	, up(_up)
	, fov(_fov)
	, aspectRatio(ratio)
{
	fovScale = (float)tan(fov * 0.5f * PI / 180.0f);
	right = front.Cross(up);
}

// (0.0f, 1.0f)
Ray Camera::generateRay(const Camera& camera, float x, float y)
{
	Vec3 r = camera.right * (camera.fovScale * (x - 0.5f) * 2.0f);
	Vec3 u = camera.up * (camera.fovScale * (y - 0.5f) * 2.0f / camera.aspectRatio);
	Vec3 dir = (camera.front + r + u).Normalize();
	return Ray(camera.eye, dir);
}

void Scene::intersect(const Ray& ray, HitResult& result)
{
	HitResult testResult;
	float closestDist = TRACE_DIST;
	for (int i = 0; i < (int)entities.size(); ++i)
	{
		Entity* ent = entities[i];
		ent->intersect(ray, testResult);
		if (testResult.bHit && testResult.distance < closestDist)
		{
			closestDist = testResult.distance;
			result.bHit = testResult.bHit;
			result.entity = testResult.entity;
			result.distance = testResult.distance;
			result.normal = testResult.normal;
			result.position = testResult.position;
		}
	}
}

Color Scene::rayTrace(const Ray& ray, HitResult& hitResult, float inIOR, int depth)
{
	if (depth > TRACE_DEPTH)
		return Color(0, 0, 0);

	Color TraceColor = Color(0, 0, 0);
	intersect(ray, hitResult);
	if (!hitResult.bHit)
	{
		return Color(0, 0, 0);
	}
	else
	{
		if (hitResult.entity->type == Entity::LIGHT)
		{
			return Color(255, 255, 255);
		}
		else
		{
			for (int i = 0; i < entities.size(); ++i)
			{
				if (entities[i]->type == Entity::LIGHT)
				{
					Entity* light = entities[i];
					Vec3 L = static_cast<Sphere*>(light->shape)->center - hitResult.position;
					L.Normalize();

					// point light shadow
					float shade = 1.0f;
					if (light->shape->getType() == IShape::SPHERE)
					{
						Ray toLightRay = Ray(hitResult.position + L * DELTA, L);
						HitResult toLightHitResult;
						intersect(toLightRay, toLightHitResult);
						if (toLightHitResult.bHit && toLightHitResult.entity != light)
						{
							shade = 0.0f;
						}
					}

					if (shade > 0.0f)
					{
						// diffuse shade
						Color diffuseColor;
						Vec3 N = hitResult.normal;
						float LdotN = L * N;
						if (LdotN > 0.0f)
						{
							diffuseColor = hitResult.entity->material->color * (LdotN * shade);
							TraceColor.AddSafe(diffuseColor);
						}

						// specular shade
						Color specularColor;
						Vec3 V = ray.direction;
						Vec3 R = L - 2.0f * (L * N) * N;
						float VdotR = V * R;
						if (VdotR > 0.0f)
						{
							float spec = powf(VdotR, hitResult.entity->material->specular);
							specularColor = hitResult.entity->material->color * (spec * shade);
							TraceColor.AddSafe(specularColor);
						}
					}
				}
			}

			// reflect shade
			Color reflectColor;
			Vec3 N = hitResult.normal;
			Vec3 R = ray.direction - 2.0f * (ray.direction * N) * N;
			Ray reflectRay = Ray(hitResult.position + R * DELTA, R);
			HitResult reflectHitResult;
			reflectColor = rayTrace(reflectRay, reflectHitResult, hitResult.entity->material->IOR, depth + 1) * hitResult.entity->material->reflectance;
			TraceColor.AddSafe(reflectColor);

			// refract shade
			/*Color refractColor;
			Vec3 T = refract(N, ray.direction, inIOR, hitResult.entity->material->IOR);
			Ray refractRay = Ray(hitResult.position + T * DELTA, T);
			HitResult refractHitResult;
			refractColor = rayTrace(refractRay, refractHitResult, hitResult.entity->material->IOR, depth + 1);
			Vec3 absource = Vec3(refractColor.r, refractColor.g, refractColor.g) / 255.0f * 0.15f * (-refractHitResult.distance);
			Vec3 transparency = Vec3(expf(absource.x), expf(absource.y), expf(absource.z));
			refractColor = Color(
				(uchar)(hitResult.entity->material->color.r * transparency.x),
				(uchar)(hitResult.entity->material->color.g * transparency.y),
				(uchar)(hitResult.entity->material->color.b * transparency.z));
			TraceColor.AddSafe(refractColor);*/
		}
	}
	return TraceColor;
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

Plane::Plane(const Vec3& n, const Vec3& p)
	: normal(n), point(p), type(IShape::PLANE)
{
}

void Plane::intersect(const Ray& ray, HitResult& result)
{
	float d = this->normal * ray.direction;
	if (d < -DELTA)
	{
		float n = this->normal * this->point - this->normal * ray.origin;
		float t = n / d;
		if (t > 0.0f && t < TRACE_DIST)
		{
			result.bHit = true;
			result.distance = t;
			result.normal = this->normal;
			result.position = Ray::getPoint(ray, t);
		}
	}
}

Entity::Entity()
{
	shape = nullptr;
	material = nullptr;
}

Entity::~Entity()
{
	if(shape)
		delete shape;
	if(material)
		delete material;
}

void Entity::intersect(const Ray& ray, HitResult& result)
{
	shape->intersect(ray, result);
	if (result.bHit)
	{
		result.entity = this;
		return;
	}
}

float Math::Clamp(float a, float lhs, float rhs)
{
	if (lhs > rhs)
	{
		float tmp = lhs;
		lhs = rhs;
		rhs = tmp;
	}

	if		(a < lhs)	{ return lhs; }
	else if (a > rhs)	{ return rhs; }
	else				{ return a; }
}

float Math::Min(float a, float b)
{
	return a < b ? a : b;
}

int Math::Max(int a, int b)
{
	return a > b ? a : b;
}

float Math::Max(float a, float b)
{
	return a > b ? a : b;
}

int Math::Min(int a, int b)
{
	return a < b ? a : b;
}

PlaneV2::PlaneV2(float a, float b, float c, float d)
{
	this->a = a;
	this->b = b; 
	this->c = c;
	this->d = d;
	type = IShape::PLANE;
}

void PlaneV2::intersect(const Ray& ray, HitResult& result)
{
	Vec3 N = Vec3(a, b, c);
	float NdotD = N * ray.direction;
	if (NdotD != 0.0f)
	{
		float NdotP = N * ray.origin;
		float t = -(NdotP + d) / NdotD;

		if (t > 0.0f && t < TRACE_DIST)
		{
			result.bHit = true;
			result.distance = t;
			result.normal = N;
			result.position = Ray::getPoint(ray, t);
		}
	}
}

Material::Material()
{
	this->color = Color(0xff, 0xff, 0xff);
	this->reflectance = 0.3f;
	this->specular = 40.0f;
	this->IOR = 1.0f;
}

Material::Material(Color color, float reflect, float specular, float refract)
{
	this->color = color;
	this->reflectance = reflect;
	this->specular = specular;
	this->IOR = reflect;
}
