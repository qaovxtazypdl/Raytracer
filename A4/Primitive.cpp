#include "Primitive.hpp"
#include "Material.hpp"
#include <iostream>
#include <algorithm>
#include <glm/ext.hpp>

using namespace glm;
using namespace std;

const double EPSILON = 1E-10;

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

IntersectionInfo NonhierSphere::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, const MaterialPackage &matpack) {
  dvec4 a = ray_origin;
  dvec4 b = ray_dir + ray_origin;
  dvec4 c = dvec4(m_pos, 1.0);
  double r = m_radius;

  double roots[2];
  size_t numRoots = quadraticRoots(dot(b-a, b-a), 2*dot(b-a, a-c), dot(c-a, c-a)-r*r, roots);

  if (numRoots != 2 || (roots[0] < EPSILON && roots[1] < EPSILON)) {
    return IntersectionInfo();
  } else {
    double t = std::min(roots[0], roots[1]);
    double t_2 = std::max(roots[0], roots[1]);

    dvec4 pt1 = t*ray_dir + ray_origin;
    dvec4 pt2 = t_2*ray_dir + ray_origin;

    dvec3 d1 = dvec3(pt1 - dvec4(m_pos, 1.0));
    dvec3 d2 = dvec3(pt2 - dvec4(m_pos, 1.0));

    double u1 = atan2(d1[2], d1[0]) / (2*PI) + 0.5;
    double u2 = atan2(d2[2], d2[0]) / (2*PI) + 0.5;
    double v1 = acos(d1[1])/PI;
    double v2 = acos(d2[1])/PI;
    dvec4 Ou1, Ou2, Ov1, Ov2;

    Ou1 = dvec4(-sin(PI*(v1))*sin(2*PI*(u1-0.5)),0,sin(PI*(v1))*cos(2*PI*(u1-0.5)),0);
    Ov1 = dvec4(cos(2*PI*(u1-0.5))*cos(PI*(v1)), -sin(PI*(v1)), sin(2*PI*(u1-0.5))*cos(PI*(v1)),0);
    Ou2 = dvec4(-sin(PI*(v2))*sin(2*PI*(u2-0.5)),0,sin(PI*(v2))*cos(2*PI*(u2-0.5)),0);
    Ov2 = dvec4(cos(2*PI*(u2-0.5))*cos(PI*(v2)), -sin(PI*(v2)), sin(2*PI*(u2-0.5))*cos(PI*(v2)),0);

    UVPackage uvp_1 = UVPackage({u1,v1}, normalize(Ou1), normalize(Ov1));
    UVPackage uvp_2 = UVPackage({u2,v2}, normalize(Ou2), normalize(Ov2));

    return IntersectionInfo({IntersectionPoint(
      t, pt1, normalize(pt1-c), matpack, this,
      t_2, pt2, normalize(pt2-c), matpack, this,
      uvp_1, uvp_2
    )});
  }
}

//refernce: https://tavianator.com/fast-branchless-raybounding-box-intersections/
IntersectionInfo NonhierBox::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, const MaterialPackage &matpack) {
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
  if (tmin > tmax || (tmin < EPSILON && tmax < EPSILON)) {
    return IntersectionInfo();
  } else {
    vector<IntersectionPoint> result;

    double u1=0, u2=0, v1=0, v2=0;
    dvec4 Ou1, Ou2, Ov1, Ov2;
    dvec3 pt;

    dvec4 normalmin;
    dvec4 intersectionmin = tmin*ray_dir + ray_origin;
    pt = (dvec3(intersectionmin) - m_pos) / m_size / 3;
    if (tmin == tx_first) {
      //left face
      u1=pt[2]; v1=pt[1]+1.0/3;
      normalmin = dvec4(-1,0,0,0);
      Ou1 = dvec4(0,0,1,0);
      Ov1 = dvec4(0,1,0,0);
    } else if (tmin == tx_second) {
      //right face
      u1=-pt[2]+1.0/2; v1=pt[1]+1.0/3;
      normalmin = dvec4(1,0,0,0);
      Ou1 = dvec4(0,0,-1,0);
      Ov1 = dvec4(0,1,0,0);
    } else if (tmin == ty_first) {
      //bottom face
      u1=pt[0]+1.0/4; v1=pt[2];
      normalmin = dvec4(0,-1,0,0);
      Ou1 = dvec4(1,0,0,0);
      Ov1 = dvec4(0,0,1,0);
    } else if (tmin == ty_second) {
      //top face
      u1=pt[0]+1.0/4; v1=-pt[2]+2.0/3;
      normalmin = dvec4(0,1,0,0);
      Ou1 = dvec4(1,0,0,0);
      Ov1 = dvec4(0,0,-1,0);
    } else if (tmin == tz_first) {
      //back face
      u1=-pt[0]+3.0/4; v1=pt[1]+1.0/3;
      normalmin = dvec4(0,0,-1,0);
      Ou1 = dvec4(-1,0,0,0);
      Ov1 = dvec4(0,1,0,0);
    } else if (tmin == tz_second) {
      //front face
      u1=pt[0]+1.0/4; v1=pt[1]+1.0/3;
      normalmin = dvec4(0,0,1,0);
      Ou1 = dvec4(1,0,0,0);
      Ov1 = dvec4(0,1,0,0);
    }

    dvec4 normalmax;
    dvec4 intersectionmax = tmax*ray_dir + ray_origin;
    pt = (dvec3(intersectionmax) - m_pos) / m_size / 3.0;
    if (tmax == tx_first) {
      //left face
      u2=pt[2]; v2=pt[1]+1.0/3;
      normalmax = dvec4(-1,0,0,0);
      Ou2 = dvec4(0,0,1,0);
      Ov2 = dvec4(0,1,0,0);
    } else if (tmax == tx_second) {
      //right face
      u2=-pt[2]+1.0/2; v2=pt[1]+1.0/3;
      normalmax = dvec4(1,0,0,0);
      Ou2 = dvec4(0,0,-1,0);
      Ov2 = dvec4(0,1,0,0);
    } else if (tmax == ty_first) {
      //bottom face
      u2=pt[0]+1.0/4; v2=pt[2];
      normalmax = dvec4(0,-1,0,0);
      Ou2 = dvec4(1,0,0,0);
      Ov2 = dvec4(0,0,1,0);
    } else if (tmax == ty_second) {
      //top face
      u2=pt[0]+1.0/4; v2=-pt[2]+2.0/3;
      normalmax = dvec4(0,1,0,0);
      Ou2 = dvec4(1,0,0,0);
      Ov2 = dvec4(0,0,-1,0);
    } else if (tmax == tz_first) {
      //back face
      u2=-pt[0]+3.0/4; v2=pt[1]+1.0/3;
      normalmax = dvec4(0,0,-1,0);
      Ou2 = dvec4(-1,0,0,0);
      Ov2 = dvec4(0,1,0,0);
    } else if (tmax == tz_second) {
      //front face
      u2=pt[0]+1.0/4; v2=pt[1]+1.0/3;
      normalmax = dvec4(0,0,1,0);
      Ou2 = dvec4(1,0,0,0);
      Ov2 = dvec4(0,1,0,0);
    }

    UVPackage uvp_1 = UVPackage({u1,v1}, Ou1, Ov1);
    UVPackage uvp_2 = UVPackage({u2,v2}, Ou2, Ov2);

    return IntersectionInfo({IntersectionPoint(
      tmin, intersectionmin, normalmin, matpack, this,
      tmax, intersectionmax, normalmax, matpack, this,
      uvp_1, uvp_2
    )});
  }
}

IntersectionInfo Cone::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, const MaterialPackage &matpack) {
  IntersectionPoint result;

  double v=0, u=0;
  dvec4 Ou, Ov;
  UVPackage uvp;
  dvec4 norm;
  dvec4 intersect;
  double roots[2];
  double tplane = (-1.0 - ray_origin[1]) / ray_dir[1];
  intersect = ray_origin + tplane*ray_dir;
  if (length(dvec3(intersect[0], intersect[2], 0)) < 1) {
    u = intersect[0]/2 + 0.5;
    v = intersect[2]/2 + 0.5;
    Ou = dvec4(1,0,0,0);
    Ov = dvec4(0,0,1,0);
    uvp = UVPackage({u,v}, normalize(Ou), normalize(Ov));
    result.addIntersection(tplane, tplane*ray_dir + ray_origin, dvec4(0,-1,0,0), matpack, this, uvp);
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
      v = intersect[1] + 1;
      u = atan2(intersect[2], intersect[0]) / (2*PI) + 0.5;
      Ov = -dvec4(-cos(2*PI*(u-0.5)),1,-sin(2*PI*(u-0.5)), 0);
      Ou = -dvec4(-(1-v)*sin(2*PI*(u-0.5)),0,(1-v)*cos(2*PI*(u-0.5)),0);
      uvp = UVPackage({u,v}, normalize(Ou), normalize(Ov));
      result.addIntersection(t_1, t_1*ray_dir + ray_origin, normalize(dvec4(intersect[0],-intersect[1],intersect[2],0)), matpack, this, uvp);
    }

    intersect = ray_origin + t_2*ray_dir;
    if (intersect[1] >= -1 && intersect[1] < -EPSILON) {
      v = intersect[1] + 1;
      u = atan2(intersect[2], intersect[0]) / (2*PI) + 0.5;
      Ov = -dvec4(-cos(2*PI*(u-0.5)),1,-sin(2*PI*(u-0.5)), 0);
      Ou = -dvec4(-(1-v)*sin(2*PI*(u-0.5)),0,(1-v)*cos(2*PI*(u-0.5)),0);
      uvp = UVPackage({u,v}, normalize(Ou), normalize(Ov));
      result.addIntersection(t_2, t_2*ray_dir + ray_origin, normalize(dvec4(intersect[0],-intersect[1],intersect[2],0)), matpack, this, uvp);
    }
  }

  return IntersectionInfo({result});
}

IntersectionInfo Cylinder::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, const MaterialPackage &matpack) {
  IntersectionPoint result;

  dvec4 norm;
  dvec4 intersect;
  double roots[2];
  double v=0, u=0;
  dvec4 Ou, Ov;
  UVPackage uvp;

  //intersection with planes z=1, z=-1
  double tplaneupper = (1.0 - ray_origin[1]) / ray_dir[1];
  intersect = ray_origin + tplaneupper*ray_dir;
  if (length(dvec3(intersect[0], intersect[2], 0)) < 1) {
    //accept and update
    u = intersect[0]/2 + 0.5;
    v = intersect[2]/2 + 0.5;
    Ou = dvec4(1,0,0,0);
    Ov = dvec4(0,0,-1,0);
    uvp = UVPackage({u,v}, normalize(Ou), normalize(Ov));
    result.addIntersection(tplaneupper, tplaneupper*ray_dir + ray_origin, dvec4(0,1,0,0), matpack, this, uvp);
  }

  double tplanelower = (-1.0 - ray_origin[1]) / ray_dir[1];
  intersect = ray_origin + tplanelower*ray_dir;
  if (length(dvec3(intersect[0], intersect[2], 0)) < 1) {
    //accept and update
    u = intersect[0]/2 + 0.5;
    v = intersect[2]/2 + 0.5;
    Ou = dvec4(1,0,0,0);
    Ov = dvec4(0,0,1,0);
    uvp = UVPackage({u,v}, normalize(Ou), normalize(Ov));
    result.addIntersection(tplanelower, tplanelower*ray_dir + ray_origin, dvec4(0,-1,0,0), matpack, this, uvp);
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
      v = intersect[1]/2 + 0.5;
      u = atan2(intersect[2], intersect[0]) / (2*PI) + 0.5;
      Ov = -dvec4(0,1,0,0);
      Ou = -dvec4(-2*PI*sin(2*PI*(u-0.5)),0,2*PI*cos(2*PI*(u-0.5)),0);
      uvp = UVPackage({u,v}, normalize(Ou), normalize(Ov));
      result.addIntersection(t_1, t_1*ray_dir + ray_origin, normalize(dvec4(intersect[0],0,intersect[2],0)), matpack, this, uvp);
    }

    intersect = ray_origin + t_2*ray_dir;
    if (abs(intersect[1]) < 1){
      v = intersect[1]/2 + 0.5;
      u = atan2(intersect[2], intersect[0]) / (2*PI) + 0.5;
      Ov = -dvec4(0,1,0,0);
      Ou = -dvec4(-2*PI*sin(2*PI*(u-0.5)),0,2*PI*cos(2*PI*(u-0.5)),0);
      uvp = UVPackage({u,v}, normalize(Ou), normalize(Ov));
      result.addIntersection(t_2, t_2*ray_dir + ray_origin, normalize(dvec4(intersect[0],0,intersect[2],0)), matpack, this, uvp);
    }
  }
  return IntersectionInfo({result});
}

IntersectionInfo Hyperboloid::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, const MaterialPackage &matpack) {
  IntersectionPoint result1, result2;
  dvec4 intersect;
  dvec4 norm, pt;
  double roots[2];
  double u=0, v=0;
  dvec4 Ou, Ov;
  UVPackage uvp;
  double k = m_inner;

  vector<pair<pair<double,dvec4>, UVPackage>> ts;

  double caprad = sqrt(1.0 + k);

  double tplanetop = (1.0 - ray_origin[1]) / ray_dir[1];
  intersect = ray_origin + tplanetop*ray_dir;
  if (intersect[0]*intersect[0] + intersect[2]*intersect[2] < 1.0 + k) {
    u = intersect[0]/(2*caprad) + 0.5;
    v = intersect[2]/(2*caprad) + 0.5;
    Ov = dvec4(1,0,0,0);
    Ou = dvec4(0,0,-1,0);
    uvp = UVPackage({u,v}, normalize(Ou), normalize(Ov));
    ts.push_back({{tplanetop, dvec4(0,1,0,0)}, uvp});
  }

  double tplanebottom = (-1.0 - ray_origin[1]) / ray_dir[1];
  intersect = ray_origin + tplanebottom*ray_dir;
  if (intersect[0]*intersect[0] + intersect[2]*intersect[2] < 1.0 + k) {
    u = intersect[0]/(2*caprad) + 0.5;
    v = intersect[2]/(2*caprad) + 0.5;
    Ov = dvec4(1,0,0,0);
    Ou = dvec4(0,0,1,0);
    uvp = UVPackage({u,v}, normalize(Ou), normalize(Ov));
    ts.push_back({{tplanebottom, dvec4(0,-1,0,0)}, uvp});
  }

  size_t numRoots = quadraticRoots(
    ray_dir[0]*ray_dir[0] + ray_dir[2]*ray_dir[2] - ray_dir[1]*ray_dir[1],
    2*(ray_origin[0]*ray_dir[0] + ray_origin[2]*ray_dir[2] - ray_origin[1]*ray_dir[1]),
    ray_origin[0]*ray_origin[0] + ray_origin[2]*ray_origin[2] - ray_origin[1]*ray_origin[1] - k, roots
  );

  double t_1, t_2, t;
  //intersection with circular portion
  if (numRoots == 2) {
    t_1 = std::min(roots[0], roots[1]);
    t_2 = std::max(roots[0], roots[1]);

    intersect = ray_origin + t_1*ray_dir;
    if (abs(intersect[1]) < 1){
      v = intersect[1]/2 + 0.5;
      u = atan2(intersect[2], intersect[0]) / (2*PI) + 0.5;
      Ov = -dvec4(1/sqrt(k+2*(v-0.5)*2*(v-0.5))*2*(v-0.5)*cos(2*PI*(u-0.5)),1,1/sqrt(k+2*(v-0.5)*2*(v-0.5))*2*(v-0.5)*sin(2*PI*(u-0.5)),0);
      Ou = -dvec4(-sqrt(k+2*(v-0.5)*2*(v-0.5))*sin(2*PI*(u-0.5)),0,sqrt(k+2*(v-0.5)*2*(v-0.5))*cos(2*PI*(u-0.5)),0);
      uvp = UVPackage({u,v}, normalize(Ou), normalize(Ov));
      ts.push_back({{t_1, normalize(dvec4(intersect[0],-intersect[1],intersect[2],0))},uvp});
    }

    intersect = ray_origin + t_2*ray_dir;
    if (abs(intersect[1]) < 1){
      v = intersect[1]/2 + 0.5;
      u = atan2(intersect[2], intersect[0]) / (2*PI) + 0.5;
      Ov = -dvec4(1/sqrt(k+2*(v-0.5)*2*(v-0.5))*2*(v-0.5)*cos(2*PI*(u-0.5)),1,1/sqrt(k+2*(v-0.5)*2*(v-0.5))*2*(v-0.5)*sin(2*PI*(u-0.5)),0);
      Ou = -dvec4(-sqrt(k+2*(v-0.5)*2*(v-0.5))*sin(2*PI*(u-0.5)),0,sqrt(k+2*(v-0.5)*2*(v-0.5))*cos(2*PI*(u-0.5)),0);
      uvp = UVPackage({u,v}, normalize(Ou), normalize(Ov));
      ts.push_back({{t_2, normalize(dvec4(intersect[0],-intersect[1],intersect[2],0))},uvp});
    }
  }

  sort(ts.begin(), ts.end(), [](const pair<pair<double,dvec4>, UVPackage> &left, const pair<pair<double,dvec4>, UVPackage> &right) {
    return left.first.first < right.first.first;
  });

  if (ts.size() == 3 || ts.size() == 1) {
    cout << "HYPERBOLOID ROOT ASSUMPTION" << endl;
    throw "HYPERBOLOID ROOT ASSUMPTION";
  } else if (ts.size() == 0) {
    return IntersectionInfo();
  }

  t = ts[0].first.first;
  pt = ray_origin + ray_dir * t;
  norm = ts[0].first.second;
  result1.addIntersection(t, pt, norm, matpack, this, ts[0].second);

  t = ts[1].first.first;
  pt = ray_origin + ray_dir * t;
  norm = ts[1].first.second;
  result1.addIntersection(t, pt, norm, matpack, this, ts[1].second);

  if (ts.size() == 4) {
    t = ts[2].first.first;
    pt = ray_origin + ray_dir * t;
    norm = ts[2].first.second;
    result2.addIntersection(t, pt, norm, matpack, this, ts[2].second);

    t = ts[3].first.first;
    pt = ray_origin + ray_dir * t;
    norm = ts[3].first.second;
    result2.addIntersection(t, pt, norm, matpack, this, ts[3].second);
  }

  return IntersectionInfo({result1, result2});
}


IntersectionInfo Torus::checkRayIntersection(const glm::dvec4 &ray_origin, const glm::dvec4 &ray_dir, const MaterialPackage &matpack) {
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
    double v=0, u=0;
    UVPackage uvp;
    dvec4 pt;
    dvec4 norm;
    dvec4 Ou, Ov;

    t = ts[0];
    pt = ray_origin + ray_dir * t;
    norm = normalize(dvec4(
      4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[0],
      4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[1],
      4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[2] + 8*R*R*pt[2],
      0
    ));
    u = atan2(pt[1], pt[0]) / (2*PI) + 0.5;
    v = (asin(pt[2]/(r+1E-10))/PI + 0.5) / 2;
    if (length(dvec3(pt[0],pt[1],0)) < R) v = 1 - v;
    Ou = dvec4(-sin(2*PI*(u-0.5)),cos(2*PI*(u-0.5)),0,0);
    Ov = dvec4(-sin(v*2*PI-PI/2)*cos(2*PI*(u-0.5)),-sin(v*2*PI-PI/2)*sin(2*PI*(u-0.5)),cos(v*2*PI-PI/2),0);
    uvp = UVPackage({u,v}, normalize(Ou), normalize(Ov));
    result1.addIntersection(t, pt, norm, matpack, this, uvp);

    t = ts[1];
    pt = ray_origin + ray_dir * t;
    norm = normalize(dvec4(
      4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[0],
      4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[1],
      4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[2] + 8*R*R*pt[2],
      0
    ));
    u = atan2(pt[1], pt[0]) / (2*PI) + 0.5;
    v = (asin(pt[2]/(r+1E-10))/PI + 0.5) / 2;
    if (length(dvec3(pt[0],pt[1],0)) < R) v = 1 - v;
    Ou = dvec4(-sin(2*PI*(u-0.5)),cos(2*PI*(u-0.5)),0,0);
    Ov = dvec4(-sin(v*2*PI-PI/2)*cos(2*PI*(u-0.5)),-sin(v*2*PI-PI/2)*sin(2*PI*(u-0.5)),cos(v*2*PI-PI/2),0);
    uvp = UVPackage({u,v}, normalize(Ou), normalize(Ov));
    result1.addIntersection(t, pt, norm, matpack, this, uvp);

    if (numRoots == 4) {
      t = ts[2];
      pt = ray_origin + ray_dir * t;
      norm = normalize(dvec4(
        4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[0],
        4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[1],
        4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[2] + 8*R*R*pt[2],
        0
      ));
      u = atan2(pt[1], pt[0]) / (2*PI) + 0.5;
      v = (asin(pt[2]/(r+1E-10))/PI + 0.5) / 2;
      if (length(dvec3(pt[0],pt[1],0)) < R) v = 1 - v;
      Ou = dvec4(-sin(2*PI*(u-0.5)),cos(2*PI*(u-0.5)),0,0);
      Ov = dvec4(-sin(v*2*PI-PI/2)*cos(2*PI*(u-0.5)),-sin(v*2*PI-PI/2)*sin(2*PI*(u-0.5)),cos(v*2*PI-PI/2),0);
      uvp = UVPackage({u,v}, normalize(Ou), normalize(Ov));
      result2.addIntersection(t, pt, norm, matpack, this, uvp);

      t = ts[3];
      pt = ray_origin + ray_dir * t;
      norm = normalize(dvec4(
        4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[0],
        4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[1],
        4 * (pt[0]*pt[0] + pt[1]*pt[1] + pt[2]*pt[2] - r*r - R*R) * pt[2] + 8*R*R*pt[2],
        0
      ));
      u = atan2(pt[1], pt[0]) / (2*PI) + 0.5;
      v = (asin(pt[2]/(r+1E-10))/PI + 0.5) / 2;
      if (length(dvec3(pt[0],pt[1],0)) < R) v = 1 - v;
      Ou = dvec4(-sin(2*PI*(u-0.5)),cos(2*PI*(u-0.5)),0,0);
      Ov = dvec4(-sin(v*2*PI-PI/2)*cos(2*PI*(u-0.5)),-sin(v*2*PI-PI/2)*sin(2*PI*(u-0.5)),cos(v*2*PI-PI/2),0);
      uvp = UVPackage({u,v}, normalize(Ou), normalize(Ov));
      result2.addIntersection(t, pt, norm, matpack, this, uvp);
    }
  }

  return IntersectionInfo({result1, result2});
}




