#pragma once

#include <vector>
#include <memory>
#include "Hittable.h"

using std::shared_ptr;
using std::make_shared;

class HittableList: public Hittable {
public:
	HittableList() {}
	HittableList(shared_ptr<Hittable> obj) {
		objects.push_back(obj);
	}
	HittableList(std::vector<shared_ptr<Hittable>> vec) : objects(vec) {}

	void Add(shared_ptr<Hittable> obj) {
		objects.push_back(obj);
	}
	virtual bool Hit(const Ray& r, HitRecord& rec, double t_min, double t_max) const;
public:
	std::vector<shared_ptr<Hittable>> objects;
};

bool HittableList::Hit(const Ray& r, HitRecord& rec, double t_min, double t_max) const {
	double clostest_t = t_max;
	bool if_Hit = false;
	HitRecord rec_temp;
	for (const auto& obj : objects) {
		if (obj->Hit(r, rec_temp, t_min, clostest_t)) {
			clostest_t = rec.t;
			if_Hit = true;
			rec = rec_temp;
		}
	}
	return if_Hit;
}