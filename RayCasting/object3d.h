#pragma once
#include "ray.h"
#include "hit.h"

class Object3D
{
public:
	Object3D()
	{

	}
	~Object3D()
	{

	}
	virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;

protected:
	Material* material;
};

class Sphere : public Object3D
{
public:
	Sphere(Vec3f center, float radius, Material* material)
	{
		this->center = new Vec3f(center.x(), center.y(), center.z());
		this->radius = radius;
		this->material = material;
	}
	~Sphere()
	{
		delete center;
	}
	virtual bool intersect(const Ray &r, Hit &h, float tmin)
	{
		// Ray: R(t) = origin + direction * t
		// Sphere: | X - center | = r
		bool result = false;
		Vec3f direction = r.getDirection();
		Vec3f origin = r.getOrigin();
		
		float a = direction.Dot3(direction);
		float b = 2 * direction.Dot3(origin - (*center));
		float c = (origin - (*center)).Length() * (origin - (*center)).Length() - radius * radius;
		float delta = b * b - 4 * a * c;
		float t = INT_MAX * 1.0f;
		if (delta > 0)
		{
			float sroot = sqrt(delta);
			float t1 = (-b - sroot) * 0.5f;
			float t2 = (-b + sroot) * 0.5f;
			if (t1 >= tmin)
			{
				t = t1;
			}
			else if (t2 >= tmin)
			{
				t = t2;
			}
			if (t < h.getT())
			{
				h.set(t, material, r);
			}
			result = true;
		}
		else
		{
			result = false;
		}
		return result;
	}
protected:
	Vec3f* center;
	float radius;
};

class Group : public Object3D
{
public:
	Group(int num_objects)
	{
		this->object3ds = new Object3D* [num_objects];
		this->num_objects = num_objects;
	}
	~Group()
	{
		delete[] object3ds;
	}
	virtual bool intersect(const Ray &r, Hit &h, float tmin)
	{
		bool intersected = false;
		for (int i = 0; i < num_objects; i++)
		{
			if (object3ds[i]->intersect(r, h, tmin) == true)
			{
				intersected = true;
			}
		}
		return intersected;
	}
	void addObject(int index, Object3D *obj)
	{
		object3ds[index] = obj;
	}
protected:
	Object3D** object3ds;
	int num_objects;
};
