#pragma once

#include "ray.h"

class Material;

class HitRecord {
public:
	vec3 pos;
	double t;
	vec3 normal;
	bool front_face;
	shared_ptr<Material> mat_ptr;

	inline void SetFaceNormal(const Ray& r, const vec3& outward_normal) {
		front_face = Dot(r.Direction(), outward_normal) < 0;		//利用向外法线与光源方向求点乘，为负则向外
		normal = front_face ? outward_normal: -outward_normal;
	}
};

class Hittable {
public:
	virtual bool Hit(const Ray& r, HitRecord& rec, double t_min, double t_max) const = 0;
};