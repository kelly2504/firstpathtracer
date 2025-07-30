// firstpathtracer.cpp : Defines the entry point for the application.

#include "firstpathtracer.h"
#include <iostream>
#include <fstream>

//adding color and vector utility functions
#include "color.h"
#include "ray.h"
#include "vec3.h"
//#include "Windows.h"

using namespace std;

std::ofstream image("imagetest6.ppm");

color ray_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(1.0, 0.0, 1.0);
}

int main() {

    if (!image.is_open()) {
        std::cout << "Error: Image file not created" << std::endl;
    }

  //SetConsoleOutputCP(CP_UTF8);

    // Image -- rendered image set- up
    //iniatilising aspect ratio
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    /*int image_width = 256;
    int image_height = 256;*/

    //calculate image height and ensure that it's at least 1
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height; 


    //camera 
    auto focal_length = 1.0;

    //viewport widths less than 1 is ok since they are real valued
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto camera_center = point3(0, 0, 0); //initialising camera location

    //calculating the vectors across the horizontal and down the vertical viewport edges
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    //change in positions
    //calculating the horizontal and vertical delta vectors from pixel to pixel
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    //calculating position of upper left pixel
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;

    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


    // Render

    /*std::cout << "P3\n" << image_width <<" "<< image_height << "\n255\n";*/

    image << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << image_height - j << " " << std::flush;
        for (int i = 0; i < image_width; i++) {
            /*auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';*/

            /*auto pixel_color = color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0);*/

            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);

            auto ray_direction = pixel_center - camera_center;

            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(image, pixel_color);

            
        }
    }
    image.close();
	std::clog << "\nDone.      \n";

    return 0;
}
