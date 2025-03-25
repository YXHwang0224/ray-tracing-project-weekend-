#pragma once

#include "ray.h"
#include "hittable.h"

class Material {
public:
	//提供入射光线和碰撞信息，返回衰减向量和出射光线
	virtual bool scatter(const Ray& ray_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian :public Material {
public:
	Lambertian(const vec3& albe) : albedo(albe) {}

	virtual bool scatter(const Ray& ray_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const {
		vec3 scatter_dir = rec.normal + RandomUnitVector();
		scattered = Ray(rec.pos, scatter_dir);
		attenuation = albedo;
		return true;
	}

public:
	vec3 albedo;//反射率即衰减率
};

class Metal :public Material {
public:
	Metal(const vec3& albe, double fuz) : albedo(albe), fuzz(fuz < 1.0f ? fuz : 1.0f) {}

	virtual bool scatter(const Ray& ray_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const {
		vec3 reflected = Reflect(ray_in.Direction(), rec.normal);
		scattered = Ray(rec.pos, reflected);
		attenuation = albedo;
		return (Dot(scattered.Direction(), rec.normal) > 0.0f);//dot的值小于0认为吸收
	}

public:
	vec3 albedo;//反射率即衰减率
	double fuzz;//模糊程度，0表示不模糊
};

double Schlick(double cosine, double ref_idx) {
	auto r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class Dielectric :public Material {
public:
	Dielectric(double ri) : ref_idx(ri) {}

	virtual bool scatter(const Ray& ray_in, const HitRecord& rec, vec3& attenuation, Ray& scattered) const {
		attenuation = vec3(1.0f, 1.0f, 1.0f);			//全部反射
		double etai_over_etat = rec.front_face ? (1.0f / ref_idx) : ref_idx;
		
		vec3 unit_dir = UnitVector(ray_in.Direction());	//对入射光进行归一化
		double cos_theta = std::min(Dot(-unit_dir, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
		if (sin_theta * etai_over_etat > 1.0f) {		//在此情况视为反射
			vec3 reflected = Reflect(unit_dir, rec.normal);
			scattered = Ray(rec.pos, reflected);
			return true;
		}

		double reflect_prob = Schlick(cos_theta, etai_over_etat);		//玻璃也有反射
		if (RandomDouble() < reflect_prob)
		{
			vec3 reflected = Reflect(unit_dir, rec.normal);
			scattered = Ray(rec.pos, reflected);
			return true;
		}

		vec3 refracted = Refract(unit_dir, rec.normal, etai_over_etat);
		scattered = Ray(rec.pos, refracted);
		return true;
	}

public:
	double ref_idx;
};