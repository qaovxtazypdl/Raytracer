#include "Primitive.hpp"
#include <iostream>
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

  if (numRoots != 2 || (roots[0] < EPSILON && roots[1] < EPSILON)) {
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

  //TODO: check
  if (tmax < -EPSILON || tmin > tmax || (tmin < EPSILON && tmax < EPSILON)) {
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
  return IntersectionInfo();
  /*bool found = false;
  double t = std::numeric_limits<double>::infinity();
  dvec4 norm;
  double roots[2];

  double tplane = (-1.0 - ray_origin[1]) / ray_dir[1];
  if (tplane > EPSILON && tplane < t) {
    dvec4 intersect = ray_origin + tplane*ray_dir;
    if (length(dvec3(intersect[0], intersect[2], 0)) < 1 - EPSILON) {
      //accept and update
      found = true;
      t = tplane;
      norm = dvec4(0,-1,0,0);
    }
  }

  size_t numRoots = quadraticRoots(
    ray_dir[0]*ray_dir[0] + ray_dir[2]*ray_dir[2] - ray_dir[1]*ray_dir[1],
    2*(ray_origin[0]*ray_dir[0] + ray_origin[2]*ray_dir[2] - ray_origin[1]*ray_dir[1]),
    ray_origin[0]*ray_origin[0] + ray_origin[2]*ray_origin[2] - ray_origin[1]*ray_origin[1], roots
  );

  //intersection with circular portion
  if (numRoots == 2) {
    double t_1 = std::min(roots[0], roots[1]);
    double t_2 = std::max(roots[0], roots[1]);

    if (length(dvec3(ray_origin[0], ray_origin[2], 0)) < 1 - EPSILON) {
      t_1 = t_2;
    }

    if (t_1 > EPSILON && t_1 < t) {
      dvec4 intersect = ray_origin + t_1*ray_dir;
      if (intersect[1] >= -1 && intersect[1] < -EPSILON){
        found = true;
        t = t_1;
        norm = normalize(dvec4(intersect[0],-intersect[1],intersect[2],0));
      }
    }
  }

  if (t > max_t || !found) {
    return IntersectionInfo();
  } else {
    return IntersectionInfo(t, t*ray_dir + ray_origin, norm);
  }*/
}

IntersectionInfo Cylinder::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Material *m_material) {
  return IntersectionInfo();
  /*bool found = false;
  double t = std::numeric_limits<double>::infinity();
  dvec4 norm;
  double roots[2];

  //intersection with planes z=1, z=-1
  double tplaneupper = (1.0 - ray_origin[1]) / ray_dir[1];
  if (tplaneupper > EPSILON && tplaneupper < t) {
    dvec4 intersect = ray_origin + tplaneupper*ray_dir;
    if (length(dvec3(intersect[0], intersect[2], 0)) < 1 - EPSILON) {
      //accept and update
      found = true;
      t = tplaneupper;
      norm = dvec4(0,1,0,0);
    }
  }

  double tplanelower = (-1.0 - ray_origin[1]) / ray_dir[1];
  if (tplanelower > EPSILON && tplanelower < t) {
    dvec4 intersect = ray_origin + tplanelower*ray_dir;
    if (length(dvec3(intersect[0], intersect[2], 0)) < 1 - EPSILON) {
      //accept and update
      found = true;
      t = tplanelower;
      norm = dvec4(0,-1,0,0);
    }
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

    if (length(dvec3(ray_origin[0], ray_origin[2], 0)) < 1 - EPSILON) {
      t_1 = t_2;
    }

    if (t_1 > EPSILON && t_1 < t) {
      dvec4 intersect = ray_origin + t_1*ray_dir;
      if (abs(intersect[1]) < 1 - EPSILON){
        found = true;
        t = t_1;
        norm = normalize(dvec4(intersect[0],0,intersect[2],0));
      }
    }
  }

  if (t > max_t || !found) {
    return IntersectionInfo();
  } else {
    return IntersectionInfo(t, t*ray_dir + ray_origin, norm);
  }*/
}

IntersectionInfo Hyperboloid::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Material *m_material) {
  return IntersectionInfo();
  /*bool found = false;
  double t = std::numeric_limits<double>::infinity();
  dvec4 norm;
  double roots[2];

  double tplanetop = (1.0 - ray_origin[1]) / ray_dir[1];
  if (tplanetop > EPSILON && tplanetop < t) {
    dvec4 intersect = ray_origin + tplanetop*ray_dir;
    if (intersect[0]*intersect[0] + intersect[2]*intersect[2] < 1.0 + m_inner) {
      //accept and update
      found = true;
      t = tplanetop;
      norm = dvec4(0,1,0,0);
    }
  }

  double tplanebottom = (-1.0 - ray_origin[1]) / ray_dir[1];
  if (tplanebottom > EPSILON && tplanebottom < t) {
    dvec4 intersect = ray_origin + tplanebottom*ray_dir;
    if (intersect[0]*intersect[0] + intersect[2]*intersect[2] < 1.0 + m_inner) {
      //accept and update
      found = true;
      t = tplanebottom;
      norm = dvec4(0,-1,0,0);
    }
  }

  size_t numRoots = quadraticRoots(
    ray_dir[0]*ray_dir[0] + ray_dir[2]*ray_dir[2] - ray_dir[1]*ray_dir[1],
    2*(ray_origin[0]*ray_dir[0] + ray_origin[2]*ray_dir[2] - ray_origin[1]*ray_dir[1]),
    ray_origin[0]*ray_origin[0] + ray_origin[2]*ray_origin[2] - ray_origin[1]*ray_origin[1] - m_inner, roots
  );

  //intersection with circular portion
  if (numRoots == 2) {
    double t_1 = std::min(roots[0], roots[1]);
    double t_2 = std::max(roots[0], roots[1]);

    if (length(dvec3(ray_origin[0], ray_origin[2], 0)) < 1 - EPSILON) {
      t_1 = t_2;
    }

    if (t_1 > EPSILON && t_1 < t) {
      dvec4 intersect = ray_origin + t_1*ray_dir;
      if (abs(intersect[1]) < 1){
        found = true;
        t = t_1;
        norm = normalize(dvec4(intersect[0],-intersect[1],intersect[2],0));
      }
    }
  }

  if (t > max_t || !found) {
    return IntersectionInfo();
  } else {
    return IntersectionInfo(t, t*ray_dir + ray_origin, norm);
  }*/
}


IntersectionInfo Torus::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, Material *m_material) {
  return IntersectionInfo();
  /*double R = 1.0;
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
  bool found = false;

  if (numRoots > 0) {
    double t = std::numeric_limits<double>::infinity();
    for (int i = 0; i < numRoots; i++) {
      if (roots[i] > EPSILON && roots[i] < t) {
        t = roots[i];
        found = true;
      }
    }

    if (found) {
      dvec4 pt = ray_origin + ray_dir * t;
      dvec4 norm = normalize(dvec4(
        4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[0],
        4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[1],
        4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[2] + 8*R*R*pt[2],
        0
      ));
      return IntersectionInfo(t, pt, norm);
    } else {
      return IntersectionInfo();
    }
  } else {
    return IntersectionInfo();
  }*/
}




