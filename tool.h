#pragma once
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <random>
#include <functional>

using std::shared_ptr;
using std::make_shared;

const double indentify = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double DegreesToRadians(double degrees) {
	return degrees * pi / 180.0f;
}

inline double RandomDouble() {
	static std::uniform_real_distribution<double> distribution(0.0f, 1.0f);
	static std::mt19937 generator;
	static std::function<double()> rand_generator =
		std::bind(distribution, generator);
	return rand_generator();
}

inline double RandomDouble(double min, double max) {
	return min + (max - min) * RandomDouble();
}

inline double Clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

#include "Ray.h"
#include "vec3.h"