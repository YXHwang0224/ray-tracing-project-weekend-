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

	void WriteColor(std::ostream& out) {
		out << static_cast<int>(255.999 * e[0]) << ' '
			<< static_cast<int>(255.999 * e[1]) << ' '
			<< static_cast<int>(255.999 * e[2]) << '\n';
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

inline double dot(const vec3& u, const vec3& v) {
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.e[1] * u.e[2] - u.e[2] * v.e[1],
		u.e[2] * u.e[1] - u.e[1] * v.e[2],
		u.e[0] * u.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
	return v / v.Length();
}