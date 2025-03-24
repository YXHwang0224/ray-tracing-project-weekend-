#include <iostream>

#include "tool.h"
#include "Hittable_list.h"
#include "sphere.h"
#include "camera.h"

vec3 Ray_color(const Ray& r, const Hittable& world_Hittable) {
	HitRecord rec;
	if (world_Hittable.Hit(r,rec,0.0f,indentify)) {
		return 0.5f * (rec.normal + vec3(1.0f, 1.0f, 1.0f));
	}
	//如果没有相交，就使用渐变天蓝色
	vec3 unit_Direction = Unit_vector(r.Direction());
	auto weight = 0.5 * (unit_Direction.y() + 1.0);//为了把[-1,1]变换到[0,1]
	return (1.0f - weight) * vec3(1.0f, 1.0f, 1.0f) + weight * vec3(0.5, 0.7, 1.0);
}

int main() {
	const int image_width = 200;
	const int image_height = 100;
	const int samples_num = 100;

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	HittableList world_objects;
	world_objects.Add(make_shared<Sphere>(vec3(0.0f, 0.0f, -1.0f), 0.5f));
	world_objects.Add(make_shared<Sphere>(vec3(0.0f, -100.5f, -1.0f), 100.0f));

	Camera cam;

	for (int j = image_height - 1; j > 0; --j) {
		std::cerr << "\rScanlines remaining:" << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i) {
			vec3 color(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < samples_num; s++) {
				double u = (i + RandomDouble()) / image_width;
				double v = (j + RandomDouble()) / image_height;
				Ray r = cam.GetRay(u, v);
				color += Ray_color(r, world_objects);
			}
			color.WriteColor(std::cout, samples_num);
		}
	}
	std::cerr << "\nDone.\n";
}