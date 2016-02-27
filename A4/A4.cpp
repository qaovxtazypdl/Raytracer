#include <glm/ext.hpp>
#include <vector>
#include <limits>

#include "A4.hpp"

using namespace std;
using namespace glm;

double PI = 3.1415926535897932384;
double INF = std::numeric_limits<double>::infinity();

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


pair<GeometryNode *, IntersectionInfo> testHit(const vector<GeometryNode *> &nodes, const vec4 &ray_origin, const vec4 &ray_dir, double max_t) {
  double min_t = INF;
  GeometryNode * minNode = NULL;
  IntersectionInfo intersectionInfo;

  for (GeometryNode * node : nodes) {
    IntersectionInfo intersect = node->m_primitive->checkRayIntersection(ray_origin, ray_dir, max_t);
    if (intersect.didIntersect && intersect.intersect_t < min_t) {
      //update max
      min_t = intersect.intersect_t;
      minNode = node;
      intersectionInfo = intersect;
    }
  }

  return pair<GeometryNode *, IntersectionInfo>(minNode, intersectionInfo);
}

vec3 directLight(const vector<GeometryNode *> &nodes, double phongExponent, const vec4 &point, const vec4 &normal, const std::list<Light *> &lights) {
  vec3 color;

  for (Light * light : lights) {
    pair<GeometryNode *, IntersectionInfo> result = testHit(nodes, point, light.position - point, 1.0);
    if (result.first == NULL) {
      //no intersection => full color
      color += vec3(0.0f);
    }
  }

  return color;
}

vec4 ggReflection(const vec4 &v, const vec4 &n) {
  return v - 2 * n * (dot(v,n));
}

//origin is point
//direction is vector
vec3 trace(const vector<GeometryNode *> &nodes, const vec4 &ray_origin, const vec4 &ray_dir, const vec3 &background, const vec3 &ambient, const std::list<Light *> &lights) {
  double k_e = 0.15;

  pair<GeometryNode *, IntersectionInfo> result = testHit(nodes, ray_origin, ray_dir, INF);

  if (result.first != NULL) {
    //HIT!
    vec3 color = k_e * ambient;

    vec4 point = ray_origin +  result.second.intersect_t * ray_dir;
    vec4 normal = result.second.normal;

    PhongMaterial mat = *dynamic_cast<PhongMaterial *>(result.first->m_material);

    if (length(mat.m_kd) > 0) {
      //diffuse
      color += mat.m_kd * directLight(nodes, mat.m_shininess, point, normal, lights);
    }

    if (length(mat.m_ks) > 0) {
      //specular
      vec4 reflDirection = ggReflection(ray_dir, normal);
      color += mat.m_ks * trace(nodes, point, reflDirection, background, ambient, lights);
    }
    return color;
  } else {
    return background;
  }
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

  vector<GeometryNode *> nodes;
  for (SceneNode * scene : root->children) {
    nodes.push_back(dynamic_cast<GeometryNode *>(scene));
  }

  cout << "Progress: 0" << endl;
	for (uint y = 0; y < ny; ++y) {
		for (uint x = 0; x < nx; ++x) {
      if ((x + y*nx)*10/(ny*nx) > (x + y*nx - 1)*10/(ny*nx)) {
        cout << "Progress: " << (x + y*nx)*100/(ny*nx) << endl;
      }
      //for each pixel, find world coordinates
      vec4 ray_dir = ray_direction(ny, nx, w, h, d, x, y, eye, view, up);
      vec3 pixelColor = trace(nodes, vec4(eye, 1.0f), ray_dir, vec3(0,150,0), ambient, lights);

      for (int i = 0; i < 3; i++) {
        image(x, y, i) = pixelColor[i];
      }
		}
	}
  cout << "Progress: 100" << endl;
}
