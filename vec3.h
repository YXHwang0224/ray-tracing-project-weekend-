#pragma once
#include <iostream>

class vec3
{
public:
	vec3() :e{ 0.0f, 0.0f, 0.0f } {}
	vec3(double x, double y, double z) : e{ x,y,z } {}
	
	double x() const {
		return e[0];
	}

	double y() const {
		return e[1];
	}

	double z() const {
		return e[2];
	}

	vec3& operator+=(const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	vec3& operator*=(const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	vec3& operator /=(const double t) {
		return *this *= 1 / t;
	}

	double LengthSquared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	double Length() const {
		return sqrt(LengthSquared());
	}

	void WriteColor(std::ostream& out, int sample_num) {
		double weight = 1.0f / sample_num;
		auto r = e[0] * weight;
		auto g = e[1] * weight;
		auto b = e[2] * weight;

		out << static_cast<int>(256 * Clamp(r, 0.0f, 0.999f)) << ' '
			<< static_cast<int>(256 * Clamp(g, 0.0f, 0.999f)) << ' '
			<< static_cast<int>(256 * Clamp(b, 0.0f, 0.999f)) << '\n';
	}

public:
	double e[3];
};

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.e[0] << v.e[1] << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

//注意两种形式都得包含
inline vec3 operator*(double t, const vec3& v) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
	return t * v;
}

inline vec3 operator/(vec3 v, double t) {
	return (1 / t) * v;
}
inline vec3 operator-(const vec3& v) {
	return vec3(-v.x(), -v.y(), -v.z());
}

inline double Dot(const vec3& u, const vec3& v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline vec3 Cross(const vec3& u, const vec3& v) {
	return vec3(u.e[1] * u.e[2] - u.e[2] * v.e[1],
		u.e[2] * u.e[1] - u.e[1] * v.e[2],
		u.e[0] * u.e[1] - u.e[1] * v.e[0]);
}

inline vec3 Unit_vector(vec3 v) {
	return v / v.Length();
}