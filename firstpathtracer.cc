// firstpathtracer.cpp : Defines the entry point for the application.

#include "firstpathtracer.h"
#include <iostream>
#include <fstream>
#include <filesystem>

//adding color and vector utility functions
#include "color.h"
#include "ray.h"
#include "vec3.h"
//#include "Windows.h"

using namespace std;

std::ofstream image("..\\imagetest01.ppm");

//function to add a sphere to the scene if the ray hits within the sphere radius
bool hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center; //vector from ray origin to sphere center
    auto a = dot(r.direction(), r.direction());
    auto b = -2.0 * dot(r.direction(), oc);
    auto c = dot(oc, oc) - (radius * radius);
    auto discriminant = (b * b) - (4* a * c); //discriminant of the quadratic equation
    return (discriminant > 0); //if discriminant is positive, the ray hits the sphere

}

color ray_color(const ray& r) {
    //if the ray hits the sphere, return a color based on the hit
    if (hit_sphere(point3(0, -1, -1), 0.5 , r)) {
        return color(1, 0, 0); //return a sphere of red color 
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 2.0);
    return ((1.0 - a) * color(1.0, 1.0, 1.0)) + (a * color(0.5, 0.7, 1.0));
}

int main() {

    // if (!image.is_open()) {
    //     std::cout << "Error: Image file not created" << std::endl;
        
    // }
    std::cout << "Working directory: " << std::filesystem::current_path() << std::endl;

  //SetConsoleOutputCP(CP_UTF8);

    // Image -- rendered image set- up
    //iniatilising aspect ratio
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    //calculate image height and ensure that it's at least 1
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height; 

    //camera 
    auto focal_length = 1.0;

    //viewport widths less than 1 is ok since they are real valued
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (aspect_ratio);
    auto camera_center = point3(0, 1, 0); //initialising camera location

    //calculating the vectors across the horizontal and down the vertical viewport edges
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    //change in positions
    //calculating the horizontal and vertical delta vectors from pixel to pixel
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    //calculating position of upper left pixel
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - (viewport_u / 2) - (viewport_v / 2);
    auto pixel00_loc = viewport_upper_left + (0.5 * (pixel_delta_u + pixel_delta_v));

    // Render
    /*std::cout << "P3\n" << image_width <<" "<< image_height << "\n255\n";*/
    image << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << image_height - j << " " << std::flush;
        for (int i = 0; i < image_width; i++) {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;

            ray r(camera_center, ray_direction);
            color pixel_color = ray_color(r);

            write_color(image, pixel_color);

            // if (i == image_width / 2 && j == image_height / 2) {
            // pixel_color = color(1, 0, 0);  // Force center pixel red
            
            // write_color(image, pixel_color);
            // std::clog << "\nCenter pixel set to red.\n";
            // }


        }
    }
    image.close();
	std::clog << "\nDone.      \n";

    return 0;
}
