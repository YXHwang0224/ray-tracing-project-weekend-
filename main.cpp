#include <iostream>

#include "tool.h"
#include "material.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"

vec3 RayColor(const Ray& r, const Hittable& world_hittable, int depth) {
	HitRecord rec;

	if (depth == 0)
		return vec3(0.0f, 0.0f, 0.0f);

	if (world_hittable.Hit(r,rec,0.0001f,indentify)) {
		Ray scatterd;
		vec3 attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scatterd))
			return attenuation * RayColor(scatterd, world_hittable, depth - 1);
		return vec3(0.0f, 0.0f, 0.0f);
	}
	//如果没有相交，就使用渐变天蓝色
	vec3 unit_Direction = UnitVector(r.Direction());
	auto weight = 0.5 * (unit_Direction.y() + 1.0);//为了把[-1,1]变换到[0,1]
	return (1.0f - weight) * vec3(1.0f, 1.0f, 1.0f) + weight * vec3(0.5, 0.7, 1.0);
}

int main() {
	const int image_width = 200;
	const int image_height = 100;
	const int samples_num = 100;
	const int depth = 100;

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	HittableList world_objects;
	world_objects.Add(make_shared<Sphere>(vec3(0, 0, -1), 0.5, make_shared<Lambertian>(vec3(0.1f, 0.2f, 0.5f))));
	world_objects.Add(make_shared<Sphere>(vec3(0, -100.5, -1), 100, make_shared<Lambertian>(vec3(0.8f, 0.8f, 0.0f))));

	world_objects.Add(make_shared<Sphere>(vec3(1, 0, -1), 0.5, make_shared<Metal>(vec3(0.8, 0.6, 0.2), 0.0f)));
	world_objects.Add(make_shared<Sphere>(vec3(-1, 0, -1), 0.5, make_shared<Dielectric>(1.5f)));
	world_objects.Add(make_shared<Sphere>(vec3(-1, 0, -1), -0.45, make_shared<Dielectric>(1.5f)));		//将球的半径设为负值, 形状看上去没变化, 但法相全都翻转到内部。来做出通透的玻璃球

	Camera cam;

	for (int j = image_height - 1; j > 0; --j) {
		std::cerr << "\rScanlines remaining:" << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			vec3 color(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < samples_num; s++) {
				double u = (i + RandomDouble()) / image_width;
				double v = (j + RandomDouble()) / image_height;
				Ray r = cam.GetRay(u, v);
				color += RayColor(r, world_objects, depth);
			}
			color.WriteColor(std::cout, samples_num);
		}
	}
	std::cerr << "\nDone.\n";
}