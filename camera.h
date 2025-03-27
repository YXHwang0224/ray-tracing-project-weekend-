#pragma once
#include "tool.h"

class Camera {
public:
	////look_from为摄像机位置，look_up为任意方向，用于求出摄像机倾斜角v，aspect为宽高比,aperture为光圈大小，focus_dist为聚焦距离
	Camera(vec3 look_from, vec3 look_at, vec3 look_up, double fov, double aspect, double aperture, double focus_dist) {	
		origin = look_from;
		lens_radius = aperture / 2.0f;

		double theta = DegreesToRadians(fov);		//fov转换为弧度制
		double half_height = tan(theta / 2.0f);		//计算fov在z轴上的投影，用fov的一半计算
		double half_width = aspect * half_height;	//计算投影
		w = UnitVector(look_from - look_at);
		u = UnitVector(Cross(look_up, w));
		v = Cross(w, u);

		lower_left_corner = origin - half_height * v * focus_dist - half_width * u * focus_dist - w * focus_dist;	//计算左下角坐标，为标准直角坐标系

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
	vec3 lower_left_corner;		//左下角
	vec3 horizontal;			//水平方向
	vec3 vertical;				//垂直方向
	vec3 u, v, w;				//三个方向的单位向量
	double lens_radius;			//光圈半径
};