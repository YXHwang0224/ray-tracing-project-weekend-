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
	
	inline static vec3 Random() {
		return vec3(RandomDouble(), RandomDouble(), RandomDouble());
	}

	inline static vec3 Random(double max, double min) {
		return vec3(RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max));
	}

	void WriteColor(std::ostream& out, int sample_num) {
		double weight = 1.0f / sample_num;
		auto r = sqrt(e[0] * weight);
		auto g = sqrt(e[1] * weight);
		auto b = sqrt(e[2] * weight);

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

//ע��������ʽ���ð���
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

inline vec3 UnitVector(vec3 v) {
	return v / v.Length();
}

//���ڽ���������������ߺͷ��߼��������߼���
inline vec3 Reflect(const vec3& v, const vec3& n) {
	return v - 2.0f * Dot(v, n) * n;
}

//���������
inline vec3 Refract(const vec3& v, const vec3& n, double etai_over_etat) {
	double cos_theta = Dot(-v, n);
	vec3 out_parallel = etai_over_etat * (v + cos_theta * n);
	vec3 out_perp = -sqrt(1.0f - out_parallel.LengthSquared()) * n;
	return out_parallel + out_perp;
}

//������ϻ�ͷ��߷������, ���Ҽ�С���ʻ��������䷽�����ȥ
inline vec3 RandomInUintSphere() {
	while (true){
		vec3 temp = vec3::Random(-1.0f, 1.0f);
		if (temp.LengthSquared() >= 1.0f)
			continue;
		return temp;
	}
}

//�ٵĹ��߻ᳯ�ŷ��߷���ɢ��
inline vec3 RandomUnitVector() {
	auto a = RandomDouble(0, 2 * pi);		//��0��2������һ������
	auto z = RandomDouble(-1, 1);			//zֵ��-1��1֮��
	auto r = sqrt(1 - z * z);				//��֤Ϊ��λ����
	return vec3(r * cos(a), r * sin(a), z);
}

//��õķ�������ȫ�������һ���Ƕȣ����ж����Ƿ�ָ�������
inline vec3 RandomInHemisphere(const vec3& normal) {
	vec3 in_unit_sphere = RandomInUintSphere();
	if (Dot(in_unit_sphere, normal) > 0.0f)
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}