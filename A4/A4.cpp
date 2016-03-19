#include "A4.hpp"

using namespace std;
using namespace glm;

//fovy radians
dvec4 proj_point(double nx, double ny, double w, double h, double d, double x, double y, const dvec3 &eye, const dvec3 &view, const dvec3 &up, int eye_offset_multiplier) {
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

  return p;
}

dvec3 directLight(const FlatPrimitives &nodes, bool isLight, const dvec3 &kd, const dvec3 &ks, double shininess, const dvec4 &v_eye, const dvec4 &point, const dvec4 &normal, const std::list<Light *> &lights) {
  dvec3 color;

  if (!isLight) {
     for (Light * light : lights) {
      color += light->lightColor(nodes, kd, ks, shininess, v_eye, point, normal);
    }
  } else {
    color = kd;
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

dvec3 kdmult(Texture *texture, const pair<double,double> &uv) {
  if (texture == NULL) {
    return dvec3(1.0, 1.0, 1.0);
  } else {
    return texture->getColorAt(uv);
  }
}

dvec4 pertnorm(Texture *texture, int bump_channel, const UVPackage &uvp, const dvec4 &norm) {
  if (texture == NULL) {
    return norm;
  } else {
    return normalize(norm + texture->getNormPerturbance(norm, uvp, bump_channel));
  }
}

vector<dvec4> glossyRays(const dvec4 &ray_origin, const dvec4 &ray_dir, double glossiness) {
  double gloss_scale = 15.0;
  vector<dvec4> result;
  dvec4 circCenter = ray_origin + gloss_scale*ray_dir;
  pair<dvec4, dvec4> axes = Utils::generateCircleAxes(normalize(dvec3(ray_dir)));

  double length_scale = length(ray_dir);


  for (int i = 0; i < MACRO_NUM_GLOSSY_SAMPLES_RADIAL; i++) {
    for (int j = 0; j < MACRO_NUM_GLOSSY_SAMPLES_ANGULAR; j++) {

      double rsqCenter = 1.0/MACRO_NUM_GLOSSY_SAMPLES_RADIAL * i + 0.5/MACRO_NUM_GLOSSY_SAMPLES_RADIAL +
        Utils::randbtwn(-0.4*1.0/MACRO_NUM_GLOSSY_SAMPLES_RADIAL, 0.4*1.0/MACRO_NUM_GLOSSY_SAMPLES_RADIAL);
      double thetaCenter = 1.0/MACRO_NUM_GLOSSY_SAMPLES_ANGULAR * j +
        Utils::randbtwn(-0.4*1.0/MACRO_NUM_GLOSSY_SAMPLES_ANGULAR, 0.4*1.0/MACRO_NUM_GLOSSY_SAMPLES_ANGULAR);

      double r = sqrt(rsqCenter) * glossiness;
      double theta = thetaCenter * 2 * PI;

      dvec4 point = circCenter +
        length_scale * r * cos(theta) * axes.first +
        length_scale * r * sin(theta) * axes.second;

      result.push_back((point - ray_origin) / gloss_scale);
    }
  }
/*
  for (int i = 0; i < MACRO_NUM_GLOSSY_SAMPLES_RADIAL*MACRO_NUM_GLOSSY_SAMPLES_ANGULAR-1; i++) {
    double theta = 2*PI/(MACRO_NUM_GLOSSY_SAMPLES_RADIAL*MACRO_NUM_GLOSSY_SAMPLES_ANGULAR-1) * i;
    dvec4 point = circCenter +
    scale * glossiness * cos(theta) * axes.first +
    scale * glossiness * sin(theta) * axes.second;

    result.push_back((point - ray_origin) / scale);
  }
  result.push_back(ray_dir);*/

  return result;
}

//origin is point
//direction is vector
dvec3 trace(const FlatPrimitives &nodes, const dvec4 &ray_origin, const dvec4 &ray_dir, const dvec3 &ambient, const std::list<Light *> &lights, double ior, int depth, int glossDepth) {
  if (depth >= 6) return getBackgroundColor(ray_origin, ray_dir, depth);
  IntersectionPoint pt = nodes.firstHitInNodeList(ray_origin, ray_dir, std::numeric_limits<double>::infinity());

  if (pt.valid) {
    dvec3 color;
    dvec4 point = pt.point_1;

    MaterialPackage &matpack = pt.m_material_1;
    PhongMaterial mat = *matpack.m_material;

    dvec4 normal = pertnorm(matpack.m_bumps, matpack.bump_channel, pt.uvp_1, pt.normal_1);
    dvec3 ks = mat.m_ks;
    dvec3 kd = mat.m_kd * kdmult(matpack.m_texture, pt.uvp_1.uv);

    double n1 = ior;
    double n2 = mat.m_indexOfRefraction;
    double opacity = mat.m_opacity;
    bool doGlossy = MACRO_GLOSSY_ON && mat.m_glossiness > 1E-5 && glossDepth < 1;
    double glossyMultiplier = 1.0/(MACRO_NUM_GLOSSY_SAMPLES_RADIAL*MACRO_NUM_GLOSSY_SAMPLES_ANGULAR);
    if (abs(n1 - n2) < 1E-10) {
      n1 = n2;
      n2 = 1.0;
    }

    pair<double, double> RT = Utils::fresnel(ray_dir, normal, n1, n2);
    RT.first = opacity + (1-opacity) * RT.first;
    RT.second = (1-opacity) * RT.second;

    color += kd * ambient;
    color += directLight(nodes, mat.isLight, kd, ks, mat.m_shininess, ray_dir, point, normal, lights);

    //reflection
    if (MACRO_REFLECTION_ON && RT.first > 0 && length(ks) > 0) {
      dvec4 reflDirection = Utils::ggReflection(ray_dir, normal);
      if (doGlossy) {
        for (const dvec4 &dir : glossyRays(point, reflDirection, mat.m_glossiness)) {
          color += glossyMultiplier * 0.5 * RT.first * ks * trace(nodes, point, dir, ambient, lights, ior, depth+1, glossDepth+1);
        }
      } else {
          color += 0.5 * RT.first * ks * trace(nodes, point, reflDirection, ambient, lights, ior, depth+1, glossDepth);
      }
    }

    if (MACRO_REFRACTION_ON && RT.second > 0) {
      dvec4 refrDirection;
      bool valid = Utils::ggRefraction(ray_dir, normal, n1, n2, refrDirection);
      if (valid) {
        if (doGlossy) {
          for (const dvec4 &dir : glossyRays(point, refrDirection, mat.m_glossiness)) {
            color += glossyMultiplier * 0.5 * RT.second * ks * trace(nodes, point, dir, ambient, lights, n2, depth+1, glossDepth+1);
          }
        } else {
            color += 0.5 * RT.second * ks * trace(nodes, point, refrDirection, ambient, lights, n2, depth+1, glossDepth);
        }

      }
    }
    return color;
  } else {
    return getBackgroundColor(ray_origin, ray_dir, depth);
  }
}

void t_trace(size_t nx, size_t ny, double w, double h, double d,
  const FlatPrimitives &nodes,
  const dvec3 & eye,
  int eye_offset_multiplier,
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
      //if (!(x == 324 && y == ny-273-1)) continue;

      if ((y + x*ny + thread_num) % num_threads != 0) continue;
      dvec4 ray_origin = dvec4(eye, 1.0) + (double)eye_offset_multiplier*dvec4(eye[2]*MACRO_3D_PARALLAX,0,0,0);
      dvec4 ray_dir = proj_point(nx, ny, w, h, d, x-1, y-1, eye, view, up, eye_offset_multiplier) - ray_origin;
      dvec3 pixelColor = trace(nodes, ray_origin, ray_dir, ambient, lights, 1.0, 0, 0);
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
  const dvec3 & eye,
  int eye_offset_multiplier,
  const dvec3 & view,
  const dvec3 & up,
  const dvec3 & ambient,
  const std::list<Light *> & lights,
  const vector<dvec3> &vertexColors,
  vector<dvec3> &colorBuffer,
  int &count,
  mutex &counterLock,
  int thread_num,
  int num_threads
) {
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
              dvec4 ray_origin = dvec4(eye, 1.0) + (double)eye_offset_multiplier*dvec4(eye[2]*MACRO_3D_PARALLAX,0,0,0);
              dvec4 ray_dir = proj_point(nx, ny, w, h, d, x - 0.5 + (double)a/MACRO_SUPERSAMPLE_SCALE + Utils::randbtwn(0.0, 1.0/MACRO_SUPERSAMPLE_SCALE), y - 0.5 + (double)b/MACRO_SUPERSAMPLE_SCALE + Utils::randbtwn(0.0, 1.0/MACRO_SUPERSAMPLE_SCALE), eye, view, up, eye_offset_multiplier) - ray_origin;
              pixelColor += (1.0/(MACRO_SUPERSAMPLE_SCALE * MACRO_SUPERSAMPLE_SCALE)) * trace(nodes, ray_origin, ray_dir, ambient, lights, 1.0, 0, 0);
            }
          }
        }
      }

      for (int i = 0; i < 3; i++) {
        colorBuffer[(nx + 2) * (y+1) + (x+1)][i] = pixelColor[i];
      }

      counterLock.lock();
        count++;
        if (count*10/maxCount > (count-1)*10/maxCount) {
          cout << "AA - Progress: " << count*100/maxCount << endl;
        }
      counterLock.unlock();
    }
  }
}

void t_write(size_t nx, size_t ny, Image &image, const vector<dvec3> &vertexColors,  int thread_num, int num_threads) {
  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) {
      if ((y + x*ny + thread_num) % num_threads != 0) continue;
      dvec3 pixelColor = vertexColors[(nx + 2) * (y+1) + (x+1)];
      pixelColor = min(pixelColor, dvec3(1.0, 1.0, 1.0));

      image(x, ny - y - 1, 0) = pixelColor[0];
      image(x, ny - y - 1, 1) = pixelColor[1];
      image(x, ny - y - 1, 2) = pixelColor[2];
    }
  }
}

//implements Digital CCIR601 luminance
dvec3 convertToGreyscale(const dvec3 &color) {
  double luminance = 0.299*color[0] + 0.587*color[1] + 0.114*color[2];
  return dvec3(luminance, luminance, luminance);
}

void t_write_stereo(size_t nx, size_t ny, Image &image, const vector<dvec3> &left, const vector<dvec3> &right, int thread_num, int num_threads) {
  for (int y = 0; y < ny; ++y) {
    for (int x = 0; x < nx; ++x) {
      if ((y + x*ny + thread_num) % num_threads != 0) continue;
      dvec3 lpix = left[(nx + 2) * (y+1) + (x+1)];
      dvec3 rpix = right[(nx + 2) * (y+1) + (x+1)];
      lpix = min(lpix, dvec3(1.0, 1.0, 1.0));
      rpix = min(rpix, dvec3(1.0, 1.0, 1.0));
      if (MACRO_3D_GREYSCALE) {
        lpix = convertToGreyscale(lpix);
        rpix = convertToGreyscale(rpix);
      }

      if (MACRO_3D_SIDE_BY_SIDE) {
        image(x + nx, ny - y - 1, 0) = rpix[0];
        image(x + nx, ny - y - 1, 1) = rpix[1];
        image(x + nx, ny - y - 1, 2) = rpix[2];

        image(x, ny - y - 1, 0) = lpix[0];
        image(x, ny - y - 1, 1) = lpix[1];
        image(x, ny - y - 1, 2) = lpix[2];
      } else {
        image(x, ny - y - 1, 0) = lpix[0];
        image(x, ny - y - 1, 1) = rpix[1];
        image(x, ny - y - 1, 2) = rpix[2];
      }
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

  /**********************************************
  dvec3 pt(0,0.7,0);
  double r = 0.3, R = 1.0;
  double u = asin(pt[2]/r)/PI + 0.5;
  double v = atan2(pt[1], pt[0]) / (2*PI) + 0.5;
  cout << u<< " " << v << endl;
  return;
  ***********************************************/
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
	size_t nx = MACRO_3D_SIDE_BY_SIDE?image.width()/2:image.width();

  double d = 100;
  double h = 2*d*tan(fovy*PI/180/2);
  double w = (double)nx/ny * h;

  FlatPrimitives nodes = buildTreeCache(root, dmat4(1.0));

  int bufsize = (nx+2) * (ny+2);
  vector<dvec3> leftColors(bufsize);
  vector<dvec3> leftColorsBuffer(bufsize);
  vector<dvec3> rightColors(bufsize);
  vector<dvec3> rightColorsBuffer(bufsize);
  mutex counterLock;
  int trace_count = 0, aa_count = 0;

  vector<thread> trace_threads;
  for (int i = 0; i < MACRO_NUM_THREADS; i++) {
    if (MACRO_USE_ANAGLYPH) {
      trace_threads.push_back(thread(t_trace, nx, ny, w, h, d, ref(nodes), ref(eye), -1, ref(view), ref(up), ref(ambient), ref(lights), ref(leftColors), ref(trace_count), ref(counterLock), i, MACRO_NUM_THREADS));
      trace_threads.push_back(thread(t_trace, nx, ny, w, h, d, ref(nodes), ref(eye), 1, ref(view), ref(up), ref(ambient), ref(lights), ref(rightColors), ref(trace_count), ref(counterLock), i, MACRO_NUM_THREADS));
    } else {
      trace_threads.push_back(thread(t_trace, nx, ny, w, h, d, ref(nodes), ref(eye), 0, ref(view), ref(up), ref(ambient), ref(lights), ref(leftColors), ref(trace_count), ref(counterLock), i, MACRO_NUM_THREADS));
    }
  }
  for (thread &t : trace_threads) t.join();

  if (MACRO_USE_SUPERSAMPLE) {
    vector<thread> aa_threads;
    for (int i = 0; i < MACRO_NUM_THREADS; i++) {
      if (MACRO_USE_ANAGLYPH) {
        aa_threads.push_back(thread(t_aa, nx, ny, w, h, d, ref(nodes), ref(eye), -1, ref(view), ref(up), ref(ambient), ref(lights), ref(leftColors), ref(leftColorsBuffer), ref(aa_count), ref(counterLock), i, MACRO_NUM_THREADS));
        aa_threads.push_back(thread(t_aa, nx, ny, w, h, d, ref(nodes), ref(eye), 1, ref(view), ref(up), ref(ambient), ref(lights), ref(rightColors), ref(rightColorsBuffer), ref(aa_count), ref(counterLock), i, MACRO_NUM_THREADS));
      } else {
        aa_threads.push_back(thread(t_aa, nx, ny, w, h, d, ref(nodes), ref(eye), 0, ref(view), ref(up), ref(ambient), ref(lights), ref(leftColors), ref(leftColorsBuffer), ref(aa_count), ref(counterLock), i, MACRO_NUM_THREADS));
      }
    }
    for (thread &t : aa_threads) t.join();
    for (int j = 0; j < bufsize; j++) {
      leftColors[j] = leftColorsBuffer[j];
      if (MACRO_USE_ANAGLYPH) {
        rightColors[j] = rightColorsBuffer[j];
      }
    }
  }

  cout << "Writing to image..." << endl;
  vector<thread> write_threads;
  for (int i = 0; i < MACRO_NUM_THREADS; i++) {
    if (MACRO_USE_ANAGLYPH) {
      write_threads.push_back(thread(t_write_stereo, nx, ny, ref(image), ref(leftColors), ref(rightColors), i, MACRO_NUM_THREADS));
    } else {
      write_threads.push_back(thread(t_write, nx, ny, ref(image), ref(leftColors), i, MACRO_NUM_THREADS));
    }
  }
  for (thread &t : write_threads) t.join();
}

