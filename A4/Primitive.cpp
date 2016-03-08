#include "Primitive.hpp"
#include <iostream>
#include <algorithm>
#include <glm/ext.hpp>

using namespace glm;
using namespace std;

const double EPSILON = 1E-12;

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

Cube::~Cube()
{
}

NonhierSphere::~NonhierSphere()
{
}

NonhierBox::~NonhierBox()
{
}

Cone::~Cone()
{
}

Cylinder::~Cylinder()
{
}

Hyperboloid::~Hyperboloid()
{
}

Torus::~Torus()
{
}

IntersectionInfo NonhierSphere::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Material *m_material) {
  //m_pos;
  //m_radius;
  dvec4 a = ray_origin;
  dvec4 b = ray_dir + ray_origin;
  dvec4 c = dvec4(m_pos, 1.0);

  double roots[2];
  size_t numRoots = quadraticRoots(dot(b-a, b-a), 2*dot(b-a, a-c), dot(c-a, c-a)-m_radius*m_radius, roots);

  if (numRoots != 2) {
    return IntersectionInfo();
  } else {
    double t = std::min(roots[0], roots[1]);
    double t_2 = std::max(roots[0], roots[1]);

    dvec4 pt1 = t*ray_dir + ray_origin;
    dvec4 pt2 = t_2*ray_dir + ray_origin;

    return IntersectionInfo({IntersectionPoint(
      t, pt1, normalize(pt1-c), m_material, this,
      t_2, pt2, normalize(pt2-c), m_material, this)}
    );
  }
}

//refernce: https://tavianator.com/fast-branchless-raybounding-box-intersections/
IntersectionInfo NonhierBox::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Material *m_material) {
  glm::dvec4 inv_ray_dir = 1.0/ray_dir;

  double tx_first = (m_pos[0] - ray_origin[0]) * inv_ray_dir[0];
  double ty_first = (m_pos[1] - ray_origin[1]) * inv_ray_dir[1];
  double tz_first = (m_pos[2] - ray_origin[2]) * inv_ray_dir[2];

  double tx_second = (m_pos[0] + m_size[0] - ray_origin[0]) * inv_ray_dir[0];
  double ty_second = (m_pos[1] + m_size[1] - ray_origin[1]) * inv_ray_dir[1];
  double tz_second = (m_pos[2] + m_size[2] - ray_origin[2]) * inv_ray_dir[2];

  double tmin = std::max(std::max(std::min(tx_first,tx_second), std::min(ty_first,ty_second)), std::min(tz_first,tz_second));
  double tmax = std::min(std::min(std::max(tx_first,tx_second), std::max(ty_first,ty_second)), std::max(tz_first,tz_second));

  if (tmin > tmax) {
    return IntersectionInfo();
  } else {
    vector<IntersectionPoint> result;

    dvec4 normalmin;
    if (tmin == tx_first) normalmin = dvec4(-1,0,0,0);
    else if (tmin == tx_second) normalmin = dvec4(1,0,0,0);
    else if (tmin == ty_first) normalmin = dvec4(0,-1,0,0);
    else if (tmin == ty_second) normalmin = dvec4(0,1,0,0);
    else if (tmin == tz_first) normalmin = dvec4(0,0,-1,0);
    else if (tmin == tz_second) normalmin = dvec4(0,0,1,0);

    dvec4 normalmax;
    if (tmax == tx_first) normalmax = dvec4(-1,0,0,0);
    else if (tmax == tx_second) normalmax = dvec4(1,0,0,0);
    else if (tmax == ty_first) normalmax = dvec4(0,-1,0,0);
    else if (tmax == ty_second) normalmax = dvec4(0,1,0,0);
    else if (tmax == tz_first) normalmax = dvec4(0,0,-1,0);
    else if (tmax == tz_second) normalmax = dvec4(0,0,1,0);

    return IntersectionInfo({IntersectionPoint(
      tmin, tmin*ray_dir + ray_origin, normalmin, m_material, this,
      tmax, tmax*ray_dir + ray_origin, normalmax, m_material, this)}
    );
  }
}

IntersectionInfo Cone::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Material *m_material) {
  IntersectionPoint result;

  dvec4 norm;
  dvec4 intersect;
  double roots[2];
  double tplane = (-1.0 - ray_origin[1]) / ray_dir[1];
  intersect = ray_origin + tplane*ray_dir;
  if (length(dvec3(intersect[0], intersect[2], 0)) < 1) {
    result.addIntersection(tplane, tplane*ray_dir + ray_origin, dvec4(0,-1,0,0), m_material, this);
  }

  size_t numRoots = quadraticRoots(
    ray_dir[0]*ray_dir[0] + ray_dir[2]*ray_dir[2] - ray_dir[1]*ray_dir[1],
    2*(ray_origin[0]*ray_dir[0] + ray_origin[2]*ray_dir[2] - ray_origin[1]*ray_dir[1]),
    ray_origin[0]*ray_origin[0] + ray_origin[2]*ray_origin[2] - ray_origin[1]*ray_origin[1], roots
  );

  if (numRoots == 2) {
    double t_1 = std::min(roots[0], roots[1]);
    double t_2 = std::max(roots[0], roots[1]);

    intersect = ray_origin + t_1*ray_dir;
    if (intersect[1] >= -1 && intersect[1] < -EPSILON) {
      result.addIntersection(t_1, t_1*ray_dir + ray_origin, normalize(dvec4(intersect[0],-intersect[1],intersect[2],0)), m_material, this);
    }

    intersect = ray_origin + t_2*ray_dir;
    if (intersect[1] >= -1 && intersect[1] < -EPSILON) {
      result.addIntersection(t_2, t_2*ray_dir + ray_origin, normalize(dvec4(intersect[0],-intersect[1],intersect[2],0)), m_material, this);
    }
  }

  return IntersectionInfo({result});
}

IntersectionInfo Cylinder::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Material *m_material) {
  IntersectionPoint result;

  dvec4 norm;
  dvec4 intersect;
  double roots[2];

  //intersection with planes z=1, z=-1
  double tplaneupper = (1.0 - ray_origin[1]) / ray_dir[1];
  intersect = ray_origin + tplaneupper*ray_dir;
  if (length(dvec3(intersect[0], intersect[2], 0)) < 1) {
    //accept and update
    result.addIntersection(tplaneupper, tplaneupper*ray_dir + ray_origin, dvec4(0,1,0,0), m_material, this);
  }

  double tplanelower = (-1.0 - ray_origin[1]) / ray_dir[1];
  intersect = ray_origin + tplanelower*ray_dir;
  if (length(dvec3(intersect[0], intersect[2], 0)) < 1) {
    //accept and update
    result.addIntersection(tplanelower, tplanelower*ray_dir + ray_origin, dvec4(0,-1,0,0), m_material, this);
  }

  size_t numRoots = quadraticRoots(
    ray_dir[0]*ray_dir[0] + ray_dir[2]*ray_dir[2],
    2*(ray_origin[0]*ray_dir[0] + ray_origin[2]*ray_dir[2]),
    ray_origin[0]*ray_origin[0] + ray_origin[2]*ray_origin[2] - 1, roots
  );

  //intersection with circular portion
  if (numRoots == 2) {
    double t_1 = std::min(roots[0], roots[1]);
    double t_2 = std::max(roots[0], roots[1]);

    intersect = ray_origin + t_1*ray_dir;
    if (abs(intersect[1]) < 1){
      result.addIntersection(t_1, t_1*ray_dir + ray_origin, normalize(dvec4(intersect[0],0,intersect[2],0)), m_material, this);
    }

    intersect = ray_origin + t_2*ray_dir;
    if (abs(intersect[1]) < 1){
      result.addIntersection(t_2, t_2*ray_dir + ray_origin, normalize(dvec4(intersect[0],0,intersect[2],0)), m_material, this);
    }
  }
  return IntersectionInfo({result});
}

IntersectionInfo Hyperboloid::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Material *m_material) {
  IntersectionPoint result1, result2;
  dvec4 intersect;
  dvec4 norm, pt;
  double roots[2];

  vector<pair<double,dvec4>> ts;

  double tplanetop = (1.0 - ray_origin[1]) / ray_dir[1];
  intersect = ray_origin + tplanetop*ray_dir;
  if (intersect[0]*intersect[0] + intersect[2]*intersect[2] < 1.0 + m_inner) {
    ts.push_back({tplanetop, dvec4(0,1,0,0)});
  }

  double tplanebottom = (-1.0 - ray_origin[1]) / ray_dir[1];
  intersect = ray_origin + tplanebottom*ray_dir;
  if (intersect[0]*intersect[0] + intersect[2]*intersect[2] < 1.0 + m_inner) {
    ts.push_back({tplanebottom, dvec4(0,-1,0,0)});
  }

  size_t numRoots = quadraticRoots(
    ray_dir[0]*ray_dir[0] + ray_dir[2]*ray_dir[2] - ray_dir[1]*ray_dir[1],
    2*(ray_origin[0]*ray_dir[0] + ray_origin[2]*ray_dir[2] - ray_origin[1]*ray_dir[1]),
    ray_origin[0]*ray_origin[0] + ray_origin[2]*ray_origin[2] - ray_origin[1]*ray_origin[1] - m_inner, roots
  );

  double t_1, t_2, t;
  //intersection with circular portion
  if (numRoots == 2) {
    t_1 = std::min(roots[0], roots[1]);
    t_2 = std::max(roots[0], roots[1]);

    intersect = ray_origin + t_1*ray_dir;
    if (abs(intersect[1]) < 1){
      ts.push_back({t_1, normalize(dvec4(intersect[0],-intersect[1],intersect[2],0))});
    }

    intersect = ray_origin + t_2*ray_dir;
    if (abs(intersect[1]) < 1){
      ts.push_back({t_2, normalize(dvec4(intersect[0],-intersect[1],intersect[2],0))});
    }
  }

  sort(ts.begin(), ts.end(), [](const pair<double, dvec4> &left, const pair<double, dvec4> &right) {
    return left.first < right.first;
  });

  if (ts.size() == 3 || ts.size() == 1) {
    cout << "TORUS ROOT ASSUMPTION" << endl;
    throw "TORUS ROOT ASSUMPTION";
  } else if (ts.size() == 0) {
    return IntersectionInfo();
  }

  t = ts[0].first;
  pt = ray_origin + ray_dir * t;
  norm = ts[0].second;
  result1.addIntersection(t, pt, norm, m_material, this);

  t = ts[1].first;
  pt = ray_origin + ray_dir * t;
  norm = ts[1].second;
  result1.addIntersection(t, pt, norm, m_material, this);

  if (ts.size() == 4) {
    t = ts[2].first;
    pt = ray_origin + ray_dir * t;
    norm = ts[2].second;
    result2.addIntersection(t, pt, norm, m_material, this);

    t = ts[3].first;
    pt = ray_origin + ray_dir * t;
    norm = ts[3].second;
    result2.addIntersection(t, pt, norm, m_material, this);
  }

  return IntersectionInfo({result1, result2});
}


IntersectionInfo Torus::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Material *m_material) {
  IntersectionPoint result1, result2;

  double R = 1.0;
  double r = m_inner;
  double a = dot(ray_dir, ray_dir);
  double b = 2 * dot(ray_origin, ray_dir);
  double c = dot(ray_origin, ray_origin) - 1 - r*r - R*R;

  double c_4, c_3, c_2, c_1, c_0;
  c_0 = c*c + 4*R*R*ray_origin[2]*ray_origin[2] - 4*R*R*r*r;
  c_1 = 2*b*c + 8*R*R*ray_origin[2]*ray_dir[2];
  c_2 = b*b + 2*a*c + 4*R*R*ray_dir[2]*ray_dir[2];
  c_3 = 2*a*b;
  c_4 = a*a;

  double roots[4];
  size_t numRoots = quarticRoots(c_3/c_4, c_2/c_4, c_1/c_4, c_0/c_4, roots);

  if (numRoots == 1 || numRoots == 3) {
    cout << "TORUS ROOT ASSUMPTION" << endl;
    throw "TORUS ROOT ASSUMPTION";
  } else if (numRoots != 0) {
    vector<double> ts;
    for (int i = 0; i < numRoots; i++) {
      ts.push_back(roots[i]);
    }
    sort(ts.begin(), ts.end());

    double t;
    dvec4 pt;
    dvec4 norm;

    t = ts[0];
    pt = ray_origin + ray_dir * t;
    norm = normalize(dvec4(
      4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[0],
      4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[1],
      4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[2] + 8*R*R*pt[2],
      0
    ));
    result1.addIntersection(t, pt, norm, m_material, this);

    t = ts[1];
    pt = ray_origin + ray_dir * t;
    norm = normalize(dvec4(
      4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[0],
      4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[1],
      4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[2] + 8*R*R*pt[2],
      0
    ));
    result1.addIntersection(t, pt, norm, m_material, this);

    if (numRoots == 4) {
      t = ts[2];
      pt = ray_origin + ray_dir * t;
      norm = normalize(dvec4(
        4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[0],
        4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[1],
        4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[2] + 8*R*R*pt[2],
        0
      ));
      result2.addIntersection(t, pt, norm, m_material, this);

      t = ts[3];
      pt = ray_origin + ray_dir * t;
      norm = normalize(dvec4(
        4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[0],
        4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[1],
        4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[2] + 8*R*R*pt[2],
        0
      ));
      result2.addIntersection(t, pt, norm, m_material, this);
    }
  }

  return IntersectionInfo({result1, result2});
}




