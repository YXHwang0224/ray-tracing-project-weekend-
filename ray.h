#pragma once

#include "vec3.h"

class ray {
public:
	ray() :dir(vec3(0.0f, 0.0f, 0.0f)), orig(vec3(0.0f, 0.0f, 0.0f)) {}
	ray(const vec3& origin, const vec3& dirction) : dir(dirction), orig(origin) {}

	vec3 origin() const {
		return orig;
	}
	vec3 direction() const {
		return dir;
	}

	vec3 at(double t) {
		return orig + dir * t;
	}
public:
	vec3 dir;
	vec3 orig;
};