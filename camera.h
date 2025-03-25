#pragma once
#include "tool.h"

class Camera {
public:
	Camera(double fov, double aspect) {
		origin = vec3(0.0, 0.0, 0.0);
		auto theta = DegreesToRadians(fov);
		auto half_height = tan(theta / 2);
		auto half_width = aspect * half_height;

		lower_left_corner = vec3(-half_width, -half_height, -1.0);

		horizontal = vec3(2 * half_width, 0.0, 0.0);
		vertical = vec3(0.0, 2 * half_height, 0.0);
	}

	Ray GetRay(double u, double v) const {
		return Ray(origin, lower_left_corner + horizontal * u + vertical * v - origin);
	}

public:
	vec3 origin;
	vec3 lower_left_corner;		//左下角
	vec3 horizontal;			//水平方向
	vec3 vertical;				//垂直方向
};