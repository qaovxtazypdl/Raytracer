#include <glm/ext.hpp>
#include <vector>
#include <limits>

#include "A4.hpp"

using namespace std;
using namespace glm;

double PI = 3.1415926535897932384;
double INF = std::numeric_limits<double>::infinity();

//fovy radians
vec4 ray_direction(double nx, double ny, double w, double h, double d, float x, float y, const vec3 &eye, const vec3 &view, const vec3 &up) {
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

vec4 ggReflection(const vec4 &v, const vec4 &n) {
  return v - 2 * n * (dot(v,n));
}

vec3 directLight(const vector<GeometryNode *> &nodes, const PhongMaterial &mat, const vec4 &v_eye, const vec4 &point, const vec4 &normal, const std::list<Light *> &lights) {
  vec3 color;

  for (Light * light : lights) {
    vec4 l_dir = vec4(light->position, 1.0f) - point;
    pair<GeometryNode *, IntersectionInfo> result = testHit(nodes, point, l_dir, 1.0);

    if (result.first == NULL) {
      double d = length(l_dir);
      double attenuation = 1.0/(light->falloff[0] + light->falloff[1]*d + light->falloff[2]*d*d);

      vec4 reflDirection = ggReflection(l_dir, normal);

      double l_dot_n = dot(normal, normalize(l_dir));
      double r_dot_v = dot(v_eye, normalize(reflDirection));

      if (l_dot_n > 0)
      color += attenuation * mat.m_kd * l_dot_n * light->colour;
      if (r_dot_v > 0)
      color += attenuation * mat.m_ks * pow(r_dot_v, mat.m_shininess) * light->colour;
    }
  }
  return color;
}


vec3 getBackgroundColor(const vec4 &ray_dir) {
  vec4 normalized_dir = max(vec4(0,0,0,0), (1.0f/1.7f) * (0.7f + normalize(ray_dir)));
  return vec3(normalized_dir[1]/2,normalized_dir[0]/2,normalized_dir[2]/2);
}

//origin is point
//direction is vector
vec3 trace(const vector<GeometryNode *> &nodes, const vec4 &ray_origin, const vec4 &ray_dir, const vec3 &ambient, const std::list<Light *> &lights, int depth) {
  if (depth >= 10) return getBackgroundColor(ray_dir);

  double k_a = 0.25;
  vec3 k_emit = vec3(0.04,0.04,0.04);
  pair<GeometryNode *, IntersectionInfo> result = testHit(nodes, ray_origin, ray_dir, INF);

  if (result.first != NULL) {
    vec3 color = k_emit + k_a * ambient;
    vec4 point = ray_origin + result.second.intersect_t * ray_dir;
    vec4 normal = result.second.normal;

/*
cout << endl;
cout << result.first->m_name << endl;
cout << result.second.component << endl;
cout << result.second.intersect_t << " " + to_string(result.second.normal)<< endl;
cout << to_string(ray_origin) + " " + to_string(ray_dir) << endl;
cout << depth << " 1 " << to_string(color) << endl;
*/

    PhongMaterial mat = *dynamic_cast<PhongMaterial *>(result.first->m_material);
    vec3 k_s = mat.m_ks;
    vec3 k_d = mat.m_kd;

    color += k_d * directLight(nodes, mat, ray_dir, point, normal, lights);

    if (length(k_s) > 0) {
      vec4 reflDirection = ggReflection(ray_dir, normal);
      color += k_s * trace(nodes, point, reflDirection, ambient, lights, depth+1);
    }
    return color;
  } else {
    return getBackgroundColor(ray_dir);
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
  int supersampleScale = 3;

  vector<GeometryNode *> nodes;
  for (SceneNode * scene : root->children) {
    nodes.push_back(dynamic_cast<GeometryNode *>(scene));
  }

  cout << "Progress: 0" << endl;
	for (uint y = 0; y < ny; ++y) {
		for (uint x = 0; x < nx; ++x) {
      if ((x + y*nx)*100/(ny*nx) > (x + y*nx - 1)*100/(ny*nx)) {
        cout << "Progress: " << (x + y*nx)*100/(ny*nx) << endl;
      }
      //if (!(x == 179 && y == ny - 293 - 1)) continue;
      //cout << x << " " << y << endl;
      //for each pixel, find world coordinates
      vec3 pixelColor;

      for (int a = -supersampleScale/2; a <= supersampleScale/2; a++) {
        for (int b = -supersampleScale/2; b <= supersampleScale/2; b++) {
          vec4 ray_dir = ray_direction(ny, nx, w, h, d, x + 0.5f + (float)a/(supersampleScale/2+1), y + 0.5f + (float)b/(supersampleScale/2+1), eye, view, up);
          pixelColor += (1.0f/(supersampleScale * supersampleScale)) * trace(nodes, vec4(eye, 1.0f), ray_dir, ambient, lights, 0);
        }
      }

      pixelColor = min(pixelColor, vec3(1.0f, 1.0f, 1.0f));

      for (int i = 0; i < 3; i++) {
        image(x, ny - y - 1, i) = pixelColor[i];
      }
		}
	}
  cout << "Progress: 100" << endl;
}
