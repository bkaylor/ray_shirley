#include <iostream>
#include <fstream>
#include "ray.h"

using namespace std;

float hit_sphere(const vec3& center, float radius, const ray&r)
{
    vec3 oc = r.origin() - center;

    float a, b, c, discriminant;
    a = dot(r.direction(), r.direction());
    b = 2.0 * dot(oc, r.direction());
    c = dot(oc, oc) - radius*radius;
    discriminant = b*b - 4*a*c;

    float ret;
    if (discriminant < 0)
    {
        ret = -1.0;
    }
    else
    {
        ret = ( -b - sqrt(discriminant) ) / (2.0*a);
    }

    return ret;
}

vec3 color(ray& r)
{
    float t = hit_sphere(vec3(0, 0, -1), 0.5, r);

    vec3 ret;

    if (t > 0.0)
    {
        vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0, 0, -1));
        ret = (0.5*vec3(N.x()+1, N.y()+1, N.z()+1));
    }
    else
    {
        vec3 unit_direction = unit_vector( r.direction() );
        t = 0.5 * (unit_direction.y() + 1.0);
        ret = ( (1.0 - t) * vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0) );
    }

    return ret;
}

int main(int argc, char *argv[])
{
    int nx = 40;
    int ny = 20;

    ofstream file;
    file.open("img.ppm");

    file << "P3\n" << nx << " " << ny << "\n255\n";

    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);

    for (int j = ny - 1; j >= 0; --j)
    {
        for (int i = 0; i < nx; ++i)
        {
            float u, v;
            u = float(i) / float(nx);
            v = float(j) / float(ny);

            ray r(origin, lower_left_corner + u*horizontal + v*vertical);

            vec3 col = color(r);

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
