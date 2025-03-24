#pragma once

#include "vec3.h"

class Ray {
public:
	Ray() :dir(vec3(0.0f, 0.0f, 0.0f)), orig(vec3(0.0f, 0.0f, 0.0f)) {}
	Ray(const vec3& Origin, const vec3& dirction) : dir(dirction), orig(Origin) {}

	vec3 Origin() const {
		return orig;
	}
	vec3 Direction() const {
		return dir;
	}

	vec3 At(double t) const {
		return orig + dir * t;
	}
public:
	vec3 dir;
	vec3 orig;
};