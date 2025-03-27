#pragma once

#include "hittable.h"

class Sphere :public Hittable {
public:
	Sphere() {}
	Sphere(vec3 cen, double rad, shared_ptr<Material> mat) : center(cen), radius(rad), mat_ptr(mat) {};

	virtual bool Hit(const Ray& r, HitRecord& rec, double t_min, double t_max) const;

public:
	vec3 center;
	double radius;
	shared_ptr<Material> mat_ptr;
};

bool Sphere::Hit(const Ray& r, HitRecord& rec, double t_min, double t_max) const {
	vec3 oc = r.Origin() - center;
	double a = r.Direction().LengthSquared();//圆心到光源的距离平方
	double half_b = Dot(oc, r.Direction());
	double c = oc.LengthSquared() - radius * radius;//求出理论切线长平方
	auto discriminant = half_b * half_b - a * c;//求根公式

	if (discriminant > 0) {
		double res_left = (- half_b - sqrt(discriminant)) / a;
		if (res_left<t_max && res_left>t_min) {
			rec.t = res_left;
			rec.pos = r.At(rec.t);
			vec3 outward_normal = (rec.pos - center) / radius;
			rec.SetFaceNormal(r, outward_normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
		double res_right = (- half_b + sqrt(discriminant)) / a;
		if (res_right<t_max && res_right>t_min) {
			rec.t = res_right;
			rec.pos = r.At(rec.t);
			vec3 outward_normal = (rec.pos - center) / radius;
			rec.SetFaceNormal(r, outward_normal);
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}
