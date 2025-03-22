#include "ray.h"
#include <iostream>

bool hit_sphere(const vec3& center, double radius, const ray& r) {
	vec3 oc = r.origin() - center;
	double a = r.direction().LengthSquared();//Բ�ĵ���Դ�ľ���ƽ��
	double b = 2.0f * dot(oc, r.direction());//
	double c = oc.LengthSquared() - radius * radius;//����������߳�ƽ��
	auto discriminant = b * b - 4 * a * c;//�����ʽ
	return (abs(discriminant) <= 0.01f);
}

vec3 ray_color(const ray& r) {
	if (hit_sphere(vec3(0, 0, -1), 0.5, r))
		return vec3(1, 0, 0);
	vec3 unit_direction = unit_vector(r.direction());
	auto weight = 0.5 * (unit_direction.y() + 1.0);//Ϊ�˰�[-1,1]�任��[0,1]
	return (1.0f - weight) * vec3(1.0f, 1.0f, 1.0f) + weight * vec3(0.5, 0.7, 1.0);
}

int main() {
	const int image_width = 200;
	const int image_height = 100;

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);//���½���ɫ
	vec3 horizonal(4.0f, 0.0f, 0.0f);//ˮƽ����任
	vec3 vertical(0.0f, 2.0f, 0.0f);//��ֱ����任
	vec3 origin(0.0f, 0.0f, 0.0f);//ԭ��

	for (int j = image_height - 1; j > 0; --j) {
		std::cerr << "\rScanlines remaining:" << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			auto u = double(i) / image_width;
			auto v = double(j) / image_height;
			ray r(origin, lower_left_corner + u * horizonal + v * vertical);
			vec3 color = ray_color(r);
			color.WriteColor(std::cout);
		}
	}
	std::cerr << "\nDone.\n";
}