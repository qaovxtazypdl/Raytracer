#include <glm/ext.hpp>

#include "A4.hpp"

using namespace std;
using namespace glm;

double PI = 3.1415926535897932384;

//fovy radians
vec4 ray_direction(double nx, double ny, double w, double h, double d, uint x, uint y, const vec3 &eye, const vec3 &view, const vec3 &up) {
  vec4 p(x, y, 0, 1);

  //translate_1  -n_x/2, -n_y/2 d
  p = translate(vec3(-nx/2, -ny/2, d)) * p;

  //scale
  p = scale(vec3(-w/nx, h/ny, 1)) * p;

  // rotate to superimpose
  vec3 w_3 = normalize(view - eye);
  vec3 u_3 = normalize(cross(up, w_3));
  vec3 v_3 = cross(w_3, u_3);
  vec4 col4 = vec4(0.0f,0.0f,0.0f,1.0f);

  p = mat4(vec4(u_3, 0.0f), vec4(v_3, 0.0f), vec4(w_3, 0.0f), col4) * p;

  //translate to lookfrom
  p = translate(vec3(eye[0], eye[1], eye[2])) * p;

  return normalize(p - vec4(eye, 1.0f));
}

//origin is point
//direction is vector
void trace(const vec4 &origin, const vec4 &direction) {

}

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const vec3 & eye,
		const vec3 & view,
		const vec3 & up,
		double fovy,

		// Lighting parameters
		const vec3 & ambient,
		const std::list<Light *> & lights
) {
  std::cout << "Calling A4_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << to_string(eye) << std::endl <<
		  "\t" << "view: " << to_string(view) << std::endl <<
		  "\t" << "up:   " << to_string(up) << std::endl <<
		  "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << to_string(ambient) << std::endl <<
		  "\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;


	size_t ny = image.height();
	size_t nx = image.width();

  double d = 100;
  double h = 2*d*tan(fovy*PI/180/2);
  double w = nx/ny * h;

	for (uint y = 0; y < ny; ++y) {
		for (uint x = 0; x < nx; ++x) {
      //for each pixel, find world coordinates
      //vec4 ray_dir = ray_direction(ny, nx, w, h, d, x, y, eye, view, up);


      //find ray direction and origin

      /*
			// Red: increasing from top to bottom
			image(x, y, 0) = (double)y / h;
			// Green: increasing from left to right
			image(x, y, 1) = (double)x / w;
			// Blue: in lower-left and upper-right corners
			image(x, y, 2) = ((y < h/2 && x < w/2)
						  || (y >= h/2 && x >= w/2)) ? 1.0 : 0.0;*/
		}
	}

  cout << to_string(ray_direction(ny, nx, w, h, d, 0, 0, eye, view, up)) << endl;
}
