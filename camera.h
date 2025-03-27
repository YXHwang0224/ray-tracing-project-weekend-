#pragma once
#include "tool.h"

class Camera {
public:
	////look_fromΪ�����λ�ã�look_upΪ���ⷽ����������������б��v��aspectΪ��߱�,apertureΪ��Ȧ��С��focus_distΪ�۽�����
	Camera(vec3 look_from, vec3 look_at, vec3 look_up, double fov, double aspect, double aperture, double focus_dist) {	
		origin = look_from;
		lens_radius = aperture / 2.0f;

		double theta = DegreesToRadians(fov);		//fovת��Ϊ������
		double half_height = tan(theta / 2.0f);		//����fov��z���ϵ�ͶӰ����fov��һ�����
		double half_width = aspect * half_height;	//����ͶӰ
		w = UnitVector(look_from - look_at);
		u = UnitVector(Cross(look_up, w));
		v = Cross(w, u);

		lower_left_corner = origin - half_height * v * focus_dist - half_width * u * focus_dist - w * focus_dist;	//�������½����꣬Ϊ��׼ֱ������ϵ

		horizontal = 2.0f * half_width * u * focus_dist;
		vertical = 2.0f * half_height * v * focus_dist;
	}

	Ray GetRay(double s, double t) const {
		vec3 rd = lens_radius * RandomInUnitDisk();
		vec3 offset = u * rd.x() + v * rd.y();

		return Ray(origin + offset, lower_left_corner + horizontal * s + vertical * t - origin - offset);
	}

public:
	vec3 origin;
	vec3 lower_left_corner;		//���½�
	vec3 horizontal;			//ˮƽ����
	vec3 vertical;				//��ֱ����
	vec3 u, v, w;				//��������ĵ�λ����
	double lens_radius;			//��Ȧ�뾶
};