#include "A4.hpp"

using namespace std;
using namespace glm;

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

dvec3 directLight(const FlatPrimitives &nodes, const PhongMaterial &mat, const dvec4 &v_eye, const dvec4 &point, const dvec4 &normal, const std::list<Light *> &lights) {
  dvec3 color;

  if (!mat.isLight) {
     for (Light * light : lights) {
      color += light->lightColor(nodes, mat, v_eye, point, normal);
    }
  } else {
    color = mat.m_kd;
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
dvec3 trace(const FlatPrimitives &nodes, const dvec4 &ray_origin, const dvec4 &ray_dir, const dvec3 &ambient, const std::list<Light *> &lights, int depth) {
  if (depth >= 7) return getBackgroundColor(ray_origin, ray_dir, depth);

  IntersectionPoint pt = nodes.firstHitInNodeList(ray_origin, ray_dir, std::numeric_limits<double>::infinity());

  if (pt.valid) {
    dvec3 color;
    dvec4 point = pt.point_1;
    dvec4 normal = pt.normal_1;

    PhongMaterial mat = *dynamic_cast<PhongMaterial *>(pt.m_material_1);

    dvec3 k_s = mat.m_ks;
    dvec3 k_d = mat.m_kd;

    color += k_d * ambient;

    //light contributions
    color += directLight(nodes, mat, ray_dir, point, normal, lights);

    //reflection
    if (MACRO_REFLECTION_ON && length(k_s) > 0) {
      dvec4 reflDirection = ggReflection(ray_dir, normal);
      color += 0.50 * k_s * trace(nodes, point, reflDirection, ambient, lights, depth+1);
    }

    if (MACRO_REFRACTION_ON && length(k_s) > 0) {
      //dvec4 reflDirection = ggReflection(ray_dir, normal);
      dvec4 refrDirection = ggRefraction(ray_dir, normal, 1.56);
      color += 0.50 * trace(nodes, point, refrDirection, ambient, lights, depth+1);
    }
    return color;
  } else {
    return getBackgroundColor(ray_origin, ray_dir, depth);
  }
}

void t_trace(size_t nx, size_t ny, double w, double h, double d,
  const FlatPrimitives &nodes,
  Image & image,
  const dvec3 & eye,
  const dvec3 & view,
  const dvec3 & up,
  const dvec3 & ambient,
  const std::list<Light *> & lights,
  vector<dvec3> &vertexColors,
  int &count,
  mutex &counterLock,
  int thread_num,
  int num_threads
) {
  int maxCount = (ny+2) * (nx+2);
  for (int y = 0; y < ny + 2; ++y) {
    for (int x = 0; x < nx + 2; ++x) {
      //if (!(x > 420 && x < 520 && y < ny-450-1 && y > ny-550-1)) continue;
      //if (!(x == 446 && y == ny-500-1)) continue;

      if ((y + x*ny + thread_num) % num_threads != 0) continue;
      dvec4 ray_dir = ray_direction(nx, ny, w, h, d, x-1, y-1, eye, view, up);
      dvec3 pixelColor = trace(nodes, dvec4(eye, 1.0), ray_dir, ambient, lights, 0);
      vertexColors[(nx + 2) * y + x] = pixelColor;

      counterLock.lock();
        count++;
        if (count*100/maxCount > (count-1)*100/maxCount) {
          cout << "Trace - Progress: " << count*100/maxCount << endl;
        }
      counterLock.unlock();
    }
  }
}

void t_aa(size_t nx, size_t ny, double w, double h, double d,
  const FlatPrimitives &nodes,
  Image & image,
  const dvec3 & eye,
  const dvec3 & view,
  const dvec3 & up,
  const dvec3 & ambient,
  const std::list<Light *> & lights,
  vector<dvec3> &vertexColors,
  int &count,
  mutex &counterLock,
  int thread_num,
  int num_threads
) {
  default_random_engine rng;
  uniform_real_distribution<double> ssrand(0.0, 1.0/MACRO_SUPERSAMPLE_SCALE);
  int maxCount = ny * nx;

  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) {
      if ((y + x*ny + thread_num) % num_threads != 0) continue;
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
              pixelColor += (1.0/(MACRO_SUPERSAMPLE_SCALE * MACRO_SUPERSAMPLE_SCALE)) * trace(nodes, dvec4(eye, 1.0), ray_dir, ambient, lights, 0);
            }
          }
        }
      }

      pixelColor = min(pixelColor, dvec3(1.0, 1.0, 1.0));

      for (int i = 0; i < 3; i++) {
        image(x, ny - y - 1, i) = pixelColor[i];
      }

      counterLock.lock();
        count++;
        if (count*10/maxCount > (count-1)*10/maxCount) {
          cout << "AA/Write - Progress: " << count*100/maxCount << endl;
        }
      counterLock.unlock();
    }
  }
}

void buildTreeCache(const SceneNode *root, dmat4 accumulatedTrans, vector<HierarchicalNodeInfo> &result) {
  accumulatedTrans = accumulatedTrans * root->get_transform();
  if (root->m_nodeType == NodeType::GeometryNode || root->m_nodeType == NodeType::CSGNode) {
    result.push_back(HierarchicalNodeInfo(root, accumulatedTrans));
  } else if (root->m_nodeType == NodeType::JointNode) {
    const JointNode * jointNode = static_cast<const JointNode *>(root);
    accumulatedTrans = accumulatedTrans * jointNode->get_joint_transform();
  }

  for (SceneNode * node : root->children) {
    buildTreeCache(node, accumulatedTrans, result);
  }
}

FlatPrimitives buildTreeCache(const SceneNode *root, dmat4 accumulatedTrans) {
  vector<HierarchicalNodeInfo> result;
  buildTreeCache(root, accumulatedTrans, result);
  return FlatPrimitives(result);
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

  /**********************************
  IntersectionInfo int1 = IntersectionInfo({
    IntersectionPoint(3, dvec4(), dvec4(), NULL, NULL, 3, dvec4(), dvec4(), NULL, NULL),
  });

  IntersectionInfo int2 = IntersectionInfo({
    IntersectionPoint(4, dvec4(), dvec4(), NULL, NULL, 4, dvec4(), dvec4(), NULL, NULL),
  });

  IntersectionInfo unioned = int1.UNION(int2);
  for(IntersectionPoint &p : unioned.intersections) {
    cout << p.intersect_t_1 << ":" << p.intersect_t_2 << endl;
  }
  return;
  ***********************************/
  /**********************************
  IntersectionInfo int1 = IntersectionInfo({
    IntersectionPoint(-3, dvec4(), dvec4(), NULL, NULL, 0.01, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(1, dvec4(), dvec4(), NULL, NULL, 3, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(4, dvec4(), dvec4(), NULL, NULL, 6, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(6.3, dvec4(), dvec4(), NULL, NULL, 13, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(40, dvec4(), dvec4(), NULL, NULL, 50, dvec4(), dvec4(), NULL, NULL),
  });

  IntersectionInfo int2 = IntersectionInfo({
    IntersectionPoint(2, dvec4(), dvec4(), NULL, NULL, 5, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(5.5, dvec4(), dvec4(), NULL, NULL, 5.7, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(5.77, dvec4(), dvec4(), NULL, NULL, 5.9, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(5.91, dvec4(), dvec4(), NULL, NULL, 6.03, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(7, dvec4(), dvec4(), NULL, NULL, 62.03, dvec4(), dvec4(), NULL, NULL),
  });

  IntersectionInfo int0 = IntersectionInfo();


  IntersectionInfo int5 = IntersectionInfo({
    IntersectionPoint(2.9, dvec4(), dvec4(), NULL, NULL, 3.1, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(4, dvec4(), dvec4(), NULL, NULL, 4, dvec4(), dvec4(), NULL, NULL),
  });

  IntersectionInfo int6 = IntersectionInfo({
    IntersectionPoint(3, dvec4(), dvec4(), NULL, NULL, 3, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(4, dvec4(), dvec4(), NULL, NULL, 4, dvec4(), dvec4(), NULL, NULL),
  });


  IntersectionInfo unioned = int5.INTERSECT(int6);
  for(IntersectionPoint &p : unioned.intersections) {
    cout << p.intersect_t_1 << ":" << p.intersect_t_2 << endl;
  }
  return;
  ***********************************/
  /**********************************
  IntersectionInfo int1 = IntersectionInfo({
    IntersectionPoint(-3, dvec4(), dvec4(), NULL, NULL, 0.01, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(2.1, dvec4(), dvec4(), NULL, NULL, 3, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(4, dvec4(), dvec4(), NULL, NULL, 6, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(6.3, dvec4(), dvec4(), NULL, NULL, 13, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(40, dvec4(), dvec4(), NULL, NULL, 50, dvec4(), dvec4(), NULL, NULL),
  });

  IntersectionInfo int2 = IntersectionInfo({
    IntersectionPoint(2, dvec4(), dvec4(), NULL, NULL, 5, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(5.5, dvec4(), dvec4(), NULL, NULL, 5.7, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(5.77, dvec4(), dvec4(), NULL, NULL, 5.9, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(5.91, dvec4(), dvec4(), NULL, NULL, 6.03, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(7, dvec4(), dvec4(), NULL, NULL, 62.03, dvec4(), dvec4(), NULL, NULL),
  });

  IntersectionInfo int3 = IntersectionInfo({
    IntersectionPoint(3, dvec4(), dvec4(), NULL, NULL, 5.75, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(5.85, dvec4(), dvec4(), NULL, NULL, 5.95, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(5.99, dvec4(), dvec4(), NULL, NULL, 8, dvec4(), dvec4(), NULL, NULL),
  });

  IntersectionInfo int4 = IntersectionInfo({
    IntersectionPoint(2, dvec4(), dvec4(), NULL, NULL, 6, dvec4(), dvec4(), NULL, NULL),
    IntersectionPoint(7, dvec4(), dvec4(), NULL, NULL, 9, dvec4(), dvec4(), NULL, NULL),
  });

  IntersectionInfo int0 = IntersectionInfo();

  IntersectionInfo unioned = int4.DIFFERENCE(int3);
  for(IntersectionPoint &p : unioned.intersections) {
    cout << p.intersect_t_1 << ":" << p.intersect_t_2 << endl;
  }
  return;
  ***********************************/
	size_t ny = image.height();
	size_t nx = image.width();

  double d = 100;
  double h = 2*d*tan(fovy*PI/180/2);
  double w = (double)nx/ny * h;

  FlatPrimitives nodes = buildTreeCache(root, dmat4(1.0));

  vector<dvec3> vertexColors((nx+2) * (ny+2));
  mutex counterLock;
  int trace_count = 0, aa_count = 0;

  vector<thread> trace_threads;
  for (int i = 0; i < MACRO_NUM_THREADS; i++) {
    trace_threads.push_back(thread(t_trace, nx, ny, w, h, d, ref(nodes), ref(image), ref(eye), ref(view), ref(up), ref(ambient), ref(lights), ref(vertexColors), ref(trace_count), ref(counterLock), i, MACRO_NUM_THREADS));
  }
  for (thread &t : trace_threads) t.join();

  vector<thread> aa_threads;
  for (int i = 0; i < MACRO_NUM_THREADS; i++) {
    aa_threads.push_back(thread(t_aa, nx, ny, w, h, d, ref(nodes), ref(image), ref(eye), ref(view), ref(up), ref(ambient), ref(lights), ref(vertexColors), ref(aa_count), ref(counterLock), i, MACRO_NUM_THREADS));
  }
  for (thread &t : aa_threads) t.join();
}

