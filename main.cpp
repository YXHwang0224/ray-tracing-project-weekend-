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

HittableList RandomScene() {
	HittableList world_objects;

	world_objects.Add(make_shared<Sphere>(vec3(0, -1000, 0), 1000, make_shared<Lambertian>(vec3(0.5, 0.5, 0.5))));

	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			double choose_mat = RandomDouble();
			vec3 center(a + 0.9f * RandomDouble(), 0.2f, 0.9f * RandomDouble());	//在x属于[a,0.9]，z属于[0,0.90]，y等于0.2上随机生成球心
			if ((center - vec3(4.0f, 0.2f, 0.0f)).Length() > 0.9) {
				if (choose_mat < 0.8) {
					vec3 albedo = vec3::Random() * vec3::Random();		//随机颜色
					world_objects.Add(make_shared<Sphere>(center, 0.2, make_shared<Lambertian>(albedo)));
				}
				else if(choose_mat < 0.95) {
					vec3 albedo = vec3::Random(0.5f, 1.0f);
					double fuzz = RandomDouble(0.0f, 0.5f);
					world_objects.Add(make_shared<Sphere>(center, 0.2, make_shared<Metal>(albedo, fuzz)));
				}
				else {
					world_objects.Add(make_shared<Sphere>(center, 0.2, make_shared<Dielectric>(1.5)));
				}
			}
		}
	}
	world_objects.Add(make_shared<Sphere>(vec3(0, 1, 0), 1.0, make_shared<Dielectric>(1.5)));
	world_objects.Add(make_shared<Sphere>(vec3(-4, 1, 0), 1.0, make_shared <Lambertian > (vec3(0.4, 0.2, 0.1))));
	world_objects.Add(make_shared<Sphere>(vec3(4, 1, 0), 1.0, make_shared<Metal>(vec3(0.7, 0.6, 0.5), 0.0)));

	return world_objects;
}

int main() {
	const int image_width = 200;
	const int image_height = 100;
	const int samples_num = 100;
	const int depth = 50;

	std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

	HittableList world_objects = RandomScene();

	const double aspect_ratio = double(image_width) / image_height;		//宽高比
	vec3 lookfrom(3, 3, 2);
	vec3 lookat(0, 0, -1);
	vec3 vup(0, 1, 0);
	auto dist_to_focus = (lookfrom - lookat).Length();
	auto aperture = 0.00;
	Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

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