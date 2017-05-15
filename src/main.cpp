#include <iostream>
#include <fstream>
#include <cfloat>
#include <random>
#include "camera.h"
#include "sphere.h"
#include "hitable_list.h"

using namespace std;

default_random_engine generator;
uniform_real_distribution<float> distribution(0.0, 1.0);

vec3 random_in_unit_sphere()
{
    vec3 p;

    do
    {
        p = 2.0*vec3(distribution(generator), distribution(generator) ,distribution(generator)) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);

    return p;
}

vec3 color(const ray& r, hitable *world)
{
    hit_record rec;

    vec3 ret;

    if (world->hit(r, 0.0, FLT_MAX, rec))
    {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        ret = 0.5 * color( ray(rec.p, target-rec.p), world );
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        ret = (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }

    return ret;
}

int main(int argc, char *argv[])
{
    int nx = 1000;
    int ny = 500;
    int ns = 50;


    ofstream file;
    file.open("img.ppm");

    file << "P3\n" << nx << " " << ny << "\n255\n";

    hitable *list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);

    hitable *world = new hitable_list(list, 2);

    camera cam;

    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; ++s)
            {
                float u, v;
                u = float(i + distribution(generator)) / float(nx);
                v = float(j + distribution(generator)) / float(ny);

                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world);
            }

            col /= float(ns);

            // Gamma correction.
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

            int ir, ig, ib;
            ir = int(255.99*col[0]);
            ig = int(255.99*col[1]);
            ib = int(255.99*col[2]);

            file << ir << " " << ig << " " << ib << "\n";
        }
    }

    file.close();

    return 0;
}
