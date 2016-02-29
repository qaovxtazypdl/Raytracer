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


pair<GeometryNode *, IntersectionInfo> testHit(const vector<HierarchicalNodeInfo> &nodes, const vec4 &ray_origin, const vec4 &ray_dir, double max_t) {
  double min_t = INF;
  GeometryNode * minNode = NULL;
  IntersectionInfo intersectionInfo;

  for (HierarchicalNodeInfo node : nodes) {
    mat4 T = node.mat;
    mat4 T_inv = node.inv;
    mat3 T_invtrans = node.invTranspose;

    IntersectionInfo intersect = node.geoNode->m_primitive->checkRayIntersection(T_inv * ray_origin, T_inv * ray_dir, max_t);
    if (intersect.didIntersect && intersect.intersect_t < min_t) {
      //update max

      min_t = intersect.intersect_t;
      minNode = node.geoNode;
      intersectionInfo = intersect;
      intersectionInfo.normal = normalize(vec4(T_invtrans * vec3(intersectionInfo.normal), 0.0f));
      intersectionInfo.point = T * intersectionInfo.point;
    }
  }

  return pair<GeometryNode *, IntersectionInfo>(minNode, intersectionInfo);
}

vec4 ggReflection(const vec4 &v, const vec4 &n) {
  return v - 2 * n * (dot(v,n));
}

vec3 directLight(const vector<HierarchicalNodeInfo> &nodes, const PhongMaterial &mat, const vec4 &v_eye, const vec4 &point, const vec4 &normal, const std::list<Light *> &lights) {
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
vec3 trace(const vector<HierarchicalNodeInfo> &nodes, const vec4 &ray_origin, const vec4 &ray_dir, const vec3 &ambient, const std::list<Light *> &lights, int depth) {
  if (depth >= 10) return getBackgroundColor(ray_dir);

  double k_a = 0;
  pair<GeometryNode *, IntersectionInfo> result = testHit(nodes, ray_origin, ray_dir, INF);

  if (result.first != NULL) {
    vec3 color = k_a * ambient;
    vec4 point = result.second.point;
    vec4 normal = result.second.normal;

    PhongMaterial mat = *dynamic_cast<PhongMaterial *>(result.first->m_material);
    vec3 k_s = mat.m_ks;
    vec3 k_d = mat.m_kd;

    //light contributions
    color += directLight(nodes, mat, ray_dir, point, normal, lights);

    //reflection
    if (length(k_s) > 0) {
      vec4 reflDirection = ggReflection(ray_dir, normal);
      color += k_s * trace(nodes, point, reflDirection, ambient, lights, depth+1);
    }
    return color;
  } else {
    return getBackgroundColor(ray_dir);
  }
}


void buildTreeCache(const SceneNode *root, mat4 accumulatedTrans, vector<HierarchicalNodeInfo> &result) {
  accumulatedTrans = accumulatedTrans * root->get_transform();
  if (root->m_nodeType == NodeType::GeometryNode) {
    GeometryNode * geometryNode = const_cast<GeometryNode *>(static_cast<const GeometryNode *>(root));
    result.push_back(HierarchicalNodeInfo(geometryNode, accumulatedTrans));
  } else if (root->m_nodeType == NodeType::JointNode) {
    const JointNode * jointNode = static_cast<const JointNode *>(root);
    accumulatedTrans = accumulatedTrans * jointNode->get_joint_transform();
  }

  for (SceneNode * node : root->children) {
    buildTreeCache(node, accumulatedTrans, result);
  }
}

vector<HierarchicalNodeInfo> buildTreeCache(const SceneNode *root, mat4 accumulatedTrans) {
  vector<HierarchicalNodeInfo> result;
  buildTreeCache(root, accumulatedTrans, result);
  return result;
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

  vector<HierarchicalNodeInfo> nodes = buildTreeCache(root, mat4(1.0f));

  cout << "Progress: 0" << endl;

  default_random_engine rng;
  uniform_real_distribution<double> ssrand(0.0, 1.0/MACRO_SUPERSAMPLE_SCALE);

	for (uint y = 0; y < ny; ++y) {
		for (uint x = 0; x < nx; ++x) {
      if ((x + y*nx)*100/(ny*nx) > (x + y*nx - 1)*100/(ny*nx)) {
        cout << "Progress: " << (x + y*nx)*100/(ny*nx) << endl;
      }
      //if (!(x == 106 && y == ny - 141 - 1) && !(x == 121 && y == ny - 141 - 1)) continue;
      //if (!(x == nx/2 && y == ny/2)) continue;
      //cout << endl;
      //cout << x << " " << y << endl;
      vec3 pixelColor;
      if (MACRO_USE_SUPERSAMPLE) {
        for (int a = 0; a < MACRO_SUPERSAMPLE_SCALE; a++) {
          for (int b = 0; b < MACRO_SUPERSAMPLE_SCALE; b++) {
            vec4 ray_dir = ray_direction(ny, nx, w, h, d, x + (double)a/MACRO_SUPERSAMPLE_SCALE + ssrand(rng), y + (double)b/MACRO_SUPERSAMPLE_SCALE + ssrand(rng), eye, view, up);
            pixelColor += (1.0/(MACRO_SUPERSAMPLE_SCALE * MACRO_SUPERSAMPLE_SCALE)) * trace(nodes, vec4(eye, 1.0f), ray_dir, ambient, lights, 0);
          }
        }
      } else {
        vec4 ray_dir = ray_direction(nx, ny, w, h, d, x + 0.5, y + 0.5, eye, view, up);
        pixelColor += trace(nodes, vec4(eye, 1.0f), ray_dir, ambient, lights, 0);
      }

      pixelColor = min(pixelColor, vec3(1.0f, 1.0f, 1.0f));

      for (int i = 0; i < 3; i++) {
        image(x, ny - y - 1, i) = pixelColor[i];
      }
		}
	}
  cout << "Progress: 100" << endl;
}
