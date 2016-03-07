#include "A4.hpp"

using namespace std;
using namespace glm;

double PI = 3.1415926535897932384;
double INF = std::numeric_limits<double>::infinity();

//fovy radians
dvec4 ray_direction(double nx, double ny, double w, double h, double d, double x, double y, const dvec3 &eye, const dvec3 &view, const dvec3 &up) {
  dvec4 p(x, y, 0, 1);

  //translate_1  -n_x/2, -n_y/2 d
  p = translate(dvec3(-nx/2, -ny/2, d)) * p;

  //scale
  p = scale(dvec3(-w/nx, h/ny, 1)) * p;

  // rotate to superimpose
  dvec3 w_3 = normalize(view - eye);
  dvec3 u_3 = normalize(cross(up, w_3));
  dvec3 v_3 = cross(w_3, u_3);
  dvec4 col4 = dvec4(0.0,0.0,0.0,1.0);

  p = dmat4(dvec4(u_3, 0.0), dvec4(v_3, 0.0), dvec4(w_3, 0.0), col4) * p;

  //translate to lookfrom
  p = translate(dvec3(eye[0], eye[1], eye[2])) * p;

  return p - dvec4(eye, 1.0);
}


dvec4 ggReflection(const dvec4 &v, const dvec4 &n) {
  return v - 2.0 * n * (dot(v,n));
}

dvec4 ggRefraction(const dvec4 &v, const dvec4 &n, double indexOfRefr) {
  double n1 = 1.33;
  double n2 = 1.67;

  double t_i = acos(-dot(v,n)/length(v)/length(n));
  double t_out = n1 * t_i/n2;

  if (t_out >= PI/2) return dvec4(0,0,0,0);
  return n1/n2 * v + (n1/n2*cos(t_i) - sqrt(1-pow(sin(t_out),2))) * n;
}

dvec3 directLight(const SceneNode *root, const PhongMaterial &mat, const dvec4 &v_eye, const dvec4 &point, const dvec4 &normal, const std::list<Light *> &lights) {
  dvec3 color;

  for (Light * light : lights) {
    dvec4 l_dir = dvec4(light->position, 1.0) - point;
    IntersectionInfo result = root->testHit(point, l_dir);
    IntersectionPoint pt = result.getFirstValidIntersection(1.0);

    if (!pt.valid) {
      double d = length(l_dir);
      double attenuation = 1.0/(light->falloff[0] + light->falloff[1]*d + light->falloff[2]*d*d);
      dvec4 reflDirection = ggReflection(l_dir, normal);
      double l_dot_n = dot(normal, normalize(l_dir));
      double r_dot_v = dot(normalize(v_eye), normalize(reflDirection));

      if (l_dot_n > 0)
        color += attenuation * mat.m_kd * l_dot_n * light->colour;
      if (r_dot_v > 0)
        color += attenuation * mat.m_ks * pow(r_dot_v, mat.m_shininess) * light->colour;
    }
  }
  return color;
}

dvec3 getBackgroundColor(const dvec4 &ray_origin, const dvec4 &ray_dir, int depth) {
  dvec3 color;

  double r = 100000.0;
  dvec4 a = ray_origin;
  dvec4 b = ray_dir + ray_origin;
  dvec4 c = dvec4(0,0,0,1.0);

  double roots[2];
  size_t numRoots = quadraticRoots(dot(b-a, b-a), 2*dot(b-a, a-c), dot(c-a, c-a)-r*r, roots);

  double t = std::max(roots[0], roots[1]);

  dvec4 intersectionPoint = t*ray_dir + ray_origin;
  dvec4 normalized_point = abs(normalize(intersectionPoint));

  int density = (pow(abs(dot(intersectionPoint, dvec4(1,-0.75,0,0)) / 1000), 1.7)) / 3 + 6;
  int pseudorandom = (int)((1.02*intersectionPoint[0] + 0.98*intersectionPoint[1] + 1.03*intersectionPoint[2]) * 3.237468235 + 23.23987188) % density;

  if (MACRO_STARFIELD_BACKGROUND_ON && pseudorandom == 0) {
    if (MACRO_USE_SUPERSAMPLE) {
      color += dvec3(pow(0.67, depth/1.25 + 0.79) * 2.67);
    } else {
      color += dvec3(pow(0.67, depth/1.25 + 0.79));
    }
  }
  color += dvec3(normalized_point[1]/1.42,normalized_point[0]/1.76,normalized_point[2]/3.3);
  return color;
}

//origin is point
//direction is vector
dvec3 trace(const SceneNode *root, const dvec4 &ray_origin, const dvec4 &ray_dir, const dvec3 &ambient, const std::list<Light *> &lights, int depth) {
  if (depth >= 10) return getBackgroundColor(ray_origin, ray_dir, depth);

  IntersectionInfo result = root->testHit(ray_origin, ray_dir);
  IntersectionPoint pt = result.getFirstValidIntersection(INF);

  if (pt.valid) {
    dvec3 color;
    dvec4 point = pt.point_1;
    dvec4 normal = pt.normal_1;

    PhongMaterial mat = *dynamic_cast<PhongMaterial *>(pt.m_material_1);

    dvec3 k_s = mat.m_ks;
    dvec3 k_d = mat.m_kd;

    color += k_d * ambient;

    //light contributions
    color += directLight(root, mat, ray_dir, point, normal, lights);

    //reflection
    if (MACRO_REFLECTION_ON && length(k_s) > 0) {
      dvec4 reflDirection = ggReflection(ray_dir, normal);
      color += 0.50 * k_s * trace(root, point, reflDirection, ambient, lights, depth+1);
    }

    if (MACRO_REFRACTION_ON && length(k_s) > 0) {
      //dvec4 reflDirection = ggReflection(ray_dir, normal);
      dvec4 refrDirection = ggRefraction(ray_dir, normal, 1.56);
      color += 0.50 * trace(root, point, refrDirection, ambient, lights, depth+1);
    }
    return color;
  } else {
    return getBackgroundColor(ray_origin, ray_dir, depth);
  }
}

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const dvec3 & eye,
		const dvec3 & view,
		const dvec3 & up,
		double fovy,

		// Lighting parameters
		const dvec3 & ambient,
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
  double w = (double)nx/ny * h;

  cout << "Progress: 0" << endl;

  default_random_engine rng;
  uniform_real_distribution<double> ssrand(0.0, 1.0/MACRO_SUPERSAMPLE_SCALE);

  vector<dvec3> vertexColors((nx+2) * (ny+2));

  for (int y = 0; y < ny + 2; ++y) {
    for (int x = 0; x < nx + 2; ++x) {
      if ((x + y*(nx+2))*100/((ny+2)*(nx+2)) > (x + y*(nx+2) - 1)*100/((ny+2)*(nx+2))) {
        cout << "Trace - Progress: " << (x + y*(nx+2))*100/((ny+2)*(nx+2)) << endl;
      }
      //if (!(x > 225 && x < 275 && y < ny-250 && y > ny-300)) continue;
      //if (!(x == 250 && y == ny-275-1)) continue;
      dvec4 ray_dir = ray_direction(nx, ny, w, h, d, x-1, y-1, eye, view, up);
      vertexColors[(nx + 2) * y + x] = trace(root, dvec4(eye, 1.0), ray_dir, ambient, lights, 0);
    }
  }

	for (int y = 0; y < ny; ++y) {
		for (int x = 0; x < nx; ++x) {
      if ((x + y*nx)*10/(ny*nx) > (x + y*nx - 1)*10/(ny*nx)) {
        cout << "AA - Progress: " << (x + y*nx)*100/(ny*nx) << endl;
      }
      //find points where pixel trace differences are high, and do a stochastic jitter SS on that pixel specifically.
      dvec3 pixelColor = vertexColors[(nx + 2) * (y+1) + (x+1)];

      bool adaptiveSS = false;
      double adaptiveThreshold = 0.03;
      if (MACRO_USE_SUPERSAMPLE) {
        if (length(vertexColors[(nx + 2) * (y) + (x+1)] - pixelColor) > adaptiveThreshold) {
          adaptiveSS = true;
        } else if (length(vertexColors[(nx + 2) * (y+2) + (x+1)] - pixelColor) > adaptiveThreshold) {
          adaptiveSS = true;
        } else if (length(vertexColors[(nx + 2) * (y+1) + (x)] - pixelColor) > adaptiveThreshold) {
          adaptiveSS = true;
        } else if (length(vertexColors[(nx + 2) * (y+1) + (x+2)] - pixelColor) > adaptiveThreshold) {
          adaptiveSS = true;
        }
      }

      if (MACRO_USE_SUPERSAMPLE_TEST) {
        if (adaptiveSS) {
          pixelColor = dvec3(1.0, 1.0, 1.0);
        } else {
          pixelColor = dvec3(0.0, 0.0, 0.0);
        }
      } else {
        if (MACRO_USE_SUPERSAMPLE && adaptiveSS) {
          pixelColor = dvec3(0.0, 0.0, 0.0);
          for (int a = 0; a < MACRO_SUPERSAMPLE_SCALE; a++) {
            for (int b = 0; b < MACRO_SUPERSAMPLE_SCALE; b++) {
              dvec4 ray_dir = ray_direction(nx, ny, w, h, d, x - 0.5 + (double)a/MACRO_SUPERSAMPLE_SCALE + ssrand(rng), y - 0.5 + (double)b/MACRO_SUPERSAMPLE_SCALE + ssrand(rng), eye, view, up);
              pixelColor += (1.0/(MACRO_SUPERSAMPLE_SCALE * MACRO_SUPERSAMPLE_SCALE)) * trace(root, dvec4(eye, 1.0), ray_dir, ambient, lights, 0);
            }
          }
        }
      }

      pixelColor = min(pixelColor, dvec3(1.0, 1.0, 1.0));

      for (int i = 0; i < 3; i++) {
        image(x, ny - y - 1, i) = pixelColor[i];
      }
		}
	}
  cout << "Progress: 100" << endl;
}
