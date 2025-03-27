#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <iostream>
#include <chrono>

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
	//���û���ཻ����ʹ�ý�������ɫ
	vec3 unit_Direction = UnitVector(r.Direction());
	auto weight = 0.5 * (unit_Direction.y() + 1.0);//Ϊ�˰�[-1,1]�任��[0,1]
	return (1.0f - weight) * vec3(1.0f, 1.0f, 1.0f) + weight * vec3(0.5, 0.7, 1.0);
}

HittableList RandomScene() {
	HittableList world_objects;

	world_objects.Add(make_shared<Sphere>(vec3(0, -1000, 0), 1000, make_shared<Lambertian>(vec3(0.5, 0.5, 0.5))));

	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			double choose_mat = RandomDouble();
			vec3 center(a + 0.9f * RandomDouble(), 0.2f, b + 0.9f * RandomDouble());	//��x����[a,0.9]��z����[0,0.90]��y����0.2�������������
			if ((center - vec3(4.0f, 0.2f, 0.0f)).Length() > 0.9f) {
				if (choose_mat < 0.8f) {
					vec3 albedo = vec3::Random() * vec3::Random();		//�����ɫ
					world_objects.Add(make_shared<Sphere>(center, 0.2f, make_shared<Lambertian>(albedo)));
				}
				else if(choose_mat < 0.95f) {
					vec3 albedo = vec3::Random(0.5f, 1.0f);
					double fuzz = RandomDouble(0.0f, 0.5f);
					world_objects.Add(make_shared<Sphere>(center, 0.2f, make_shared<Metal>(albedo, fuzz)));
				}
				else {
					world_objects.Add(make_shared<Sphere>(center, 0.2f, make_shared<Dielectric>(1.5f)));
				}
			}
		}
	}
	world_objects.Add(make_shared<Sphere>(vec3(0.0f, 1.0f, 0.0f), 1.0f, make_shared<Dielectric>(1.5f)));
	world_objects.Add(make_shared<Sphere>(vec3(-4.0f, 1.0f, 0.0f), 1.0f, make_shared <Lambertian> (vec3(0.4f, 0.2f, 0.1f))));
	world_objects.Add(make_shared<Sphere>(vec3(4.0f, 1.0f, 0.0f), 1.0f, make_shared<Metal>(vec3(0.7f, 0.6f, 0.5f), 0.0f)));

	return world_objects;
}

void multiThreadRender(
	int image_height, int image_width, int samples_num, int depth,
	const Camera& cam, const HittableList& world_objects
) {
	// ��ʼ����ɫ������
	std::vector<std::vector<vec3>> color_buffer(image_height);
	for (auto& row : color_buffer) row.resize(image_width);

	// ׼���������
	std::queue<int> j_queue;
	for (int j = image_height - 1; j > 0; --j) j_queue.push(j);
	std::mutex queue_mutex;
	std::atomic<int> remaining_lines = j_queue.size();

	// �����̺߳���
	auto worker = [&]() {
		while (true) {
			// ��ȡ����
			int j;
			{
				std::lock_guard<std::mutex> lock(queue_mutex);
				if (j_queue.empty()) break;
				j = j_queue.front();
				j_queue.pop();
			}

			// ������
			for (int i = 0; i < image_width; ++i) {
				vec3 color(0.0f, 0.0f, 0.0f);
				for (int s = 0; s < samples_num; ++s) {
					double u = (i + RandomDouble()) / image_width;
					double v = (j + RandomDouble()) / image_height;
					Ray r = cam.GetRay(u, v);
					color += RayColor(r, world_objects, depth);
				}
				color_buffer[j][i] = color;
			}

			// ���½���
			remaining_lines--;
		}
		};

	// �����߳�
	unsigned thread_num = std::thread::hardware_concurrency();
	std::vector<std::thread> threads;
	for (int t = 0; t < thread_num; ++t)
		threads.emplace_back(worker);

	// ������ʾ�߳�
	std::thread progress([&]() {
		while (remaining_lines > 0) {
			std::cerr << "\rScanlines remaining: " << remaining_lines << ' ' << std::flush;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		});

	// �ȴ����й����߳����
	for (auto& t : threads) t.join();
	progress.join();  // ȷ�������߳̽���
	std::cerr << "\nRender complete.\n";

	// ������
	for (int j = image_height - 1; j > 0; --j)
		for (int i = 0; i < image_width; ++i)
			color_buffer[j][i].WriteColor(std::cout, samples_num);
}

int main() {
	const int image_width = 1440;
	const int image_height = 1080;
	const int samples_num = 100;
	const int depth = 50;

	std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

	HittableList world_objects = RandomScene();

	const double aspect_ratio = double(image_width) / image_height;		//��߱�
	vec3 lookfrom(13.0f, 2.0f, 3.0f);
	vec3 lookat(0.0f, 0.0f, 0.0f);
	vec3 vup(0.0f, 1.0f, 0.0f);
	auto dist_to_focus = 10.0f;
	auto aperture = 0.01f;
	Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

	multiThreadRender(image_height, image_width, samples_num, depth, cam, world_objects);

	/*for (int j = image_height - 1; j > 0; --j) {
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
	}*/
	std::cerr << "\nDone.\n";
}